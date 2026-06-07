# bae-prep

Structured preparation for starting as a software engineer at BAE Systems
Maritime Services. This repository collects the projects, exercises, and notes I
am working through in the months before I start, rebuilding my foundations in the
three languages the role centres on — C, Ada/SPARK, and Python — with the
embedded and safety-critical focus that defines the domain.

## Why this exists

The role is embedded, safety-critical software for naval systems: code that runs
on constrained hardware, with deterministic timing, no tolerance for undefined
behaviour, and a hard requirement to fail safe rather than fail silently. That is
a different discipline from general application development, so rather than arrive
cold I am spending the lead-up deliberately practising it.

The work is organised into three phases, each anchored by a portfolio project.
The Phase I state machine is carried directly into Phase II — the same system
rebuilt in Ada and then formally verified with SPARK — so that phase is about
what the language and its tooling bring to a problem I already understand, rather
than learning a new problem at the same time. Phase III is a separate
numerical-modelling project.

A note on framing: every project here is an educational console simulation or
exercise. The defence theme keeps the work close to the domain I am entering, but
nothing in this repository is real or operational. It is practice and portfolio
code.

## Approach

- **Project-led.** Reading and exercises are scaffolding; the project is where
  the concepts have to actually fit together.
- **Spaced repetition.** Concepts I get wrong or have to look up twice become
  Anki cards; deck exports live under `notes/`.
- **A maths thread.** Linear algebra and then calculus run alongside the code to
  support the numerical work in the final phase.

## Repository structure

| Path | Purpose |
|------|---------|
| `c-state-machine/` | Phase I flagship — an embedded-style finite state machine in C modelling an arming/disarming safety sequence. Has its own README. |
| `c-programming-a-modern-approach/` | Worked exercises and programming projects from *C Programming: A Modern Approach* (K. N. King), used to rebuild core C. |
| `caesar_cipher/` | A small standalone C program (a Caesar cipher with file I/O) written as an early warm-up. |
| `ada-state-machine/` | Phase II — a port of the C state machine to Ada 2012, with SPARK contracts for formal verification (in progress). |
| `torpedo-trajectory-sim/` | Phase III — a Python numerical simulation of underwater trajectory using NumPy, SciPy, and Matplotlib (in progress). |
| `notes/` | Cross-cutting notes (such as RTOS concepts) and Anki deck exports. |

## Phases

### Phase I — C, with an embedded focus

Rebuilding C from the ground up with embedded constraints in mind: fixed-size
memory, no dynamic allocation, deterministic control flow, and fail-safe fault
handling. The flagship project is [`c-state-machine`](c-state-machine/). RTOS
concepts — task scheduling, queues, priority inversion — are studied alongside as
background for the kind of system this code would run on.

What I am taking from it: table-driven state machines, static memory discipline,
designing for the fault path first, and the general shape of bare-metal C.

### Phase II — Ada 2012 and SPARK

Porting the C state machine to Ada, then adding SPARK contracts and running the
prover to verify properties statically — the same problem, approached through a
language built for high-integrity software.

What I am taking from it: strong typing that makes illegal states
unrepresentable, design by contract (pre- and postconditions), and what it means
to prove the absence of runtime errors before the program ever runs.

### Phase III — Python for numerical modelling

Less about the language, more about the scientific ecosystem: NumPy, SciPy's ODE
solvers, and Matplotlib. The project models underwater trajectory, including
hand-written Euler and RK4 integrators compared against SciPy's solver.

What I am taking from it: numerical integration, verifying a solver against a
known analytical solution, and presenting results clearly.

## Status

Phase I is nearly complete; `c-state-machine` is the most developed project and
has its own README. The Ada and Python projects are scaffolded and will be built
out during their phases.
