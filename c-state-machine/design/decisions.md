# Design Decisions

This document records the rationale behind the significant design choices in the
C state machine, so that the *why* is preserved alongside the code rather than
lost. The system is ported to Ada 2012 with SPARK contracts in Phase II, so these
decisions carry forward directly into that work.

---

## Problem and domain

### 1. Modelling an arming / disarming safety sequence

**Decision.** The project models the arming and disarming sequence of a notional
weapon system as a finite state machine.

**Rationale.** This is the pattern that maps most directly to the safety-critical
embedded work I am starting at BAE Systems Maritime Services, and it was chosen
specifically to drill the properties that matter in defence software:
reproducibility, auditability, and safety. The core design tension is that the
system must make accidental firing impossible while still firing reliably on a
legitimate command — two requirements that pull in opposite directions, and the
state machine is how that tension is resolved.

---

## State model

### 2. A two-step ARMED → ENABLED commitment

**Decision.** Reaching the point of firing requires two distinct operator steps —
ARM (SAFE → ARMING → ARMED) and then ENABLE (ARMED → ENABLED) — before FIRE.

**Rationale.** The two states guard against human error by separating
system-readiness from operator-commitment. ARMED signals that the *system* is
ready but does not assume the *operator* is; ENABLED requires a deliberate second
commitment from the operator, bringing them to the same level of engagement as
the system before firing becomes possible.

### 3. DISARMING as an explicit state

**Decision.** Returning to SAFE passes through a dedicated DISARMING state rather
than transitioning directly.

**Rationale.** In a real system the safety mechanisms removed during arming are
physical, and they must be deliberately re-established in sequence before the
system is safe again. DISARMING models that re-engagement. It has limited
behavioural effect in a pure-software mock, but including it keeps the model
faithful to how a real arming sequence has to unwind — which is the point of the
exercise.

### 4. FIRED as the sole terminal state; a continuous control loop

**Decision.** FIRED is the only terminal state — every other state, including
FAULT, has a defined exit. The driving loop runs continuously and stops only on
reaching FIRED.

**Rationale.** Because FAULT is recoverable via RESET (see §12), FIRED is the
single genuine dead-end in the system. Modelling the driver as a loop that runs
until that terminal state — rather than a script that exits after a fixed
sequence — is closer to a real embedded superloop, which idles awaiting the next
command rather than terminating.

---

## Architecture

### 5. Two cooperating state machines

**Decision.** The system is split into two independent state machines — the main
arming sequence and a physical interlock — rather than one combined machine.

**Rationale.** A single machine would have to hold the state of both the logical
sequence and the physical mechanism together, mixing concerns and risking invalid
combined states. Separating them keeps each machine's state isolated and its
responsibility clear, which helps with refactoring, debugging, and future change.
It also permits physical separation: if one machine grew large enough to warrant
its own processor or chip, it could move without restructuring the other.

**Trade-off.** The two machines must coordinate, which is handled through a shared
buffer and router rather than direct calls (see §6).

### 6. Decoupled communication through a shared buffer

**Decision.** The two machines never call each other directly. All communication
passes through the shared event queue, which acts as the single source of truth.

**Rationale.** A direct call would couple one machine to the other's internals and
make the system far harder to audit. Routing everything through the shared buffer
means every command into each machine, and every entry/exit action that results,
can be observed in one place. Auditability is a first-order requirement here, and
the shared buffer is what makes it achievable.

### 7. A single global event queue

**Decision.** The system uses one global event queue rather than passing a queue
instance into the machines.

**Rationale.** A passed-in queue would make sense if the system needed multiple
queues or the ability to swap them, but this implementation is small enough that a
single queue meets every functional need. Keeping it global also gave one
authoritative location for all queue-related code, which made diagnosing queue
bugs significantly easier.

**Trade-off.** Less flexible than dependency-injecting a queue; acceptable given
the scale and the single-queue design.

### 8. Removal of unreachable commands

**Decision.** `ARMING_COMPLETE` and `DISARMING_COMPLETE` were removed from the
command enumeration once the interlock subsumed the arming/disarming process.

**Rationale.** Once the interlock *became* the arming process, those commands were
unreachable. A dead command left in the enumeration is a latent trap — a value
that should never be produced but could still be passed by a future caller,
creating an error that need not exist. Removing them keeps the enumeration an
accurate description of the commands the system actually uses.

---

## Transitions and failure handling

### 9. Table-driven transitions

**Decision.** Transitions are encoded as a `[state][command]` lookup table rather
than nested `switch` statements.

**Rationale.** A table turns control logic into data, which is far easier to audit
than reading through a large nested switch, and far easier to edit — changing a
transition means changing one cell, not locating the right case in a sprawl of
branches. Looking up the next state is a direct, constant-time index. (The
auditability is the primary motivation; a compiler may optimise a switch into a
jump table, so the value here is in the declarative, inspectable structure rather
than raw lookup speed.)

### 10. Reject-and-stay versus fail-safe-to-FAULT

**Decision.** The system distinguishes two failure modes. A command that is simply
not valid in the current state is rejected and the machine stays put. Anything
genuinely wrong — a corrupt state or command, or an entry action that reports
failure — drives the machine to FAULT.

**Rationale.** In a system this tightly defined, the correct behaviour is fully
specified, so anything unexpected is by definition incorrect. In a weapons
context, "incorrect" can mean an accidental firing and loss of life. It is
therefore always safer to treat genuinely unexpected behaviour as a fault and fail
safe than to ignore it and hope it was harmless. Embedded code also has no
exceptions to fall back on, so FAULT is the error channel. The two modes are kept
separate because a merely-invalid operator command is an expected, recoverable
event — not a system failure.

---

## Memory

### 11. Static memory and a fixed-size ring buffer

**Decision.** No dynamic allocation is used anywhere. The event queue is a
fixed-size ring buffer (capacity 16).

**Rationale.** On a constrained microcontroller, knowing exactly how much memory
the program uses is essential. `malloc` introduces non-determinism in both
allocation success and memory layout; a fixed ring buffer occupies the same space
on every run, which is deterministic and bounded at compile time. The capacity of
16 is a power of two (for efficient index wrapping) chosen to give comfortable
headroom over the eight commands — roughly double the realistic worst case.

**Note.** Queue depth is more precisely a function of burst behaviour than of the
number of distinct commands, but the chosen headroom is generous for this
system's event rate, so the margin is more than sufficient.

---

## Fault recovery and the interlock handshake

### 12. FAULT is recoverable, routed through DISARMING

**Decision.** FAULT is recoverable: a RESET command routes FAULT → DISARMING
rather than leaving FAULT as a terminal dead-end.

**Rationale.** This is a deliberate generalisation. Some defence systems can be
safely reset after an engineer has inspected them; others, such as a torpedo in
flight, cannot. I chose to model the recoverable case. Recovery is routed through
the full DISARMING sequence rather than snapping straight to SAFE because the
safety interlocks must be confirmed back in place before the system can be
considered safe — jumping directly to SAFE could leave the physical mechanism in a
dangerous state.

### 13. RESET re-closes the interlock; the two-machine handshake

**Decision.** On RESET from FAULT, the interlock is driven to CLOSING — actively
re-closing from wherever it is — rather than assumed to be CLOSED. (The FAULT-row
RESET transition was changed from `STATE_CLOSED` to `STATE_CLOSING` for this
reason.)

**Rationale.** The interlock could be in any position when a fault occurs —
half-open, fully open — so recovery must drive it closed from its actual state
rather than assume it is already shut. Driving it through CLOSING also keeps the
two machines in sync: in this design, the interlock reaching CLOSED is what
signals the main machine back to SAFE, so the two depend on one another through
that handshake. Assuming CLOSED would skip the CLOSING phase, the
`INTERLOCK_CLOSED` signal would never be produced, and the main machine would wait
in DISARMING indefinitely. Re-closing through CLOSING both restores the mechanism
safely and resolves that desynchronisation.

---

## Timing and the watchdog

### 14. Logical ticks instead of wall-clock time

**Decision.** Time is modelled as a logical tick counter behind a `now()` /
`clock_tick()` / `clock_init()` interface, not wall-clock time.

**Rationale.** Wall-clock time has to originate from some authoritative source, and
deciding which process owns that is an unnecessary complication that buys nothing
here. Tick counts are simpler to work with, simpler to store (a start tick and a
current tick), and can still be related back to real time through the loop
frequency when needed.

### 15. Timeouts as first-class faults

**Decision.** A state that remains active beyond its time limit is itself treated
as a fault: the watchdog raises `FAULT_DETECTED`.

**Rationale.** Missing a deadline means the system is no longer behaving
deterministically, and determinism is essential in defence software. An overrun
almost always indicates a real problem, physical or software. Rather than wait —
which could lead to a dangerous outcome — the system takes no risk and transitions
to FAULT.

### 16. Nested watchdogs: the inner timer is tighter than the outer

**Decision.** For the states that overlap in time, the interlock's watchdog limit
(500 ticks) is set tighter than the main machine's (800 ticks).

**Rationale.** While the main machine waits in ARMING, the interlock is in OPENING,
physically driving the hatch. If that motion stalls, the interlock's own watchdog
should fire first so that the resulting fault points at the actual failing
component — the interlock mechanism — rather than at the main sequence merely
observing that nothing happened. The main machine's longer limit acts as a
backstop for the case where the interlock never reports completion and somehow has
not faulted itself. This is defence in depth applied to timing: the tightest timer
sits closest to the physical event, and each looser outer timer is a fallback.

### 17. Time limits in raw ticks, not milliseconds

**Decision.** Per-state timeout values are expressed directly in ticks rather than
converted from milliseconds.

**Rationale.** The system does not need millisecond semantics, and adding a
conversion layer would be complexity for its own sake. Working in raw ticks keeps
the timeout tables simple and the relationship between a limit and the loop that
enforces it direct. Simplicity is the priority.

---

## Testing and known limitations

### 18. Tests designed to map onto SPARK contracts

**Decision.** The assert-based test harness is written so that each check
corresponds to a SPARK precondition or postcondition for the Phase II Ada/SPARK
port.

**Rationale.** Designing the tests with the port in mind from the start means the
Phase II work is largely transcription rather than redesign. Writing tests that
did not map onto the contract structure would make porting more long-winded,
converting each check into its contract form after the fact.

### 19. Known limitation: router ordering via an enum threshold

**Decision (deferred).** The router decides which machine processes an
interlock-related command first using a magnitude comparison on the command
enumeration (`cmd >= CMD_INTERLOCK_CLOSED`). This is known to be brittle and is
retained as a documented limitation rather than refactored now.

**Rationale.** The comparison depends on the ordering of the enumeration and on
there being exactly two machines; adding a third machine or reordering the commands
would break it. For the current two-machine system it is a reasonable balance of
simplicity against functionality, and it works as expected.

**Trigger for revisiting.** Introducing a third state machine. At that point the
threshold would be replaced with an explicit routing/ordering mechanism.
