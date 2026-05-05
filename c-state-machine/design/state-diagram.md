# State Diagram

## Overview

This project implements a finite state machine modelling an arm and disarm 
sequence, similar to the patterns I am expected to work with at BAE Maritime 
Services. The goal is to practise implementing real-world embedded state machine 
patterns whilst understanding the challenges faced with memory and environment 
restrictions, alongside learning more about safety protocols, RTOS, and 
schedulers — many of which are covered later in the plan.

---

## Quick Reference

| State | Triggered By | Transitions To |
|-------|-------------|----------------|
| `SAFE` | Initial state | `ARMING` |
| `ARMING` | `ARM_CMD` | `ARMED`, `FAULT`, `DISARMING` |
| `ARMED` | `ARMING_COMPLETE` | `ENABLED`, `FAULT`, `DISARMING` |
| `ENABLED` | `ENABLE_CMD` | `FIRED`, `FAULT`, `DISARMING` |
| `FIRED` | `FIRE_CMD` | — *(terminal)* |
| `FAULT` | `FAULT_DETECTED` | `DISARMING` |
| `DISARMING` | `ABORT_CMD`, `RESET_CMD` | `SAFE`, `FAULT` |

---

## States

### `SAFE` — Initial State

The initial state of the system. All safety mechanisms are active, making it 
physically impossible for the missile/torpedo to fire accidentally due to a 
hardware or mechanical error.

---

### `ARMING`

Triggered by an `ARM_CMD` issued by an operator. This begins a controlled 
process of removing each safety mechanism one by one in a specific order, rather 
than all at once — adding an additional layer of safety to the sequence.

**Possible transitions from this state:**

- If any safety mechanism fails during the process, a `FAULT_DETECTED` command 
  is issued internally and the system transitions to `FAULT`.
- If all safety mechanisms are successfully removed, `ARMING_COMPLETE` is issued 
  internally and the system transitions to `ARMED`.
- If an operator issues `ABORT_CMD` at any point, the system transitions to 
  `DISARMING`.

---

### `ARMED`

Reached when `ARMING_COMPLETE` is issued internally, confirming all safety 
mechanisms have been removed. From here, the system awaits human input — 
specifically the `ENABLE_CMD` — before proceeding to the final state prior to 
firing.

**Possible transitions from this state:**

- `ENABLE_CMD` issued by an operator → transitions to `ENABLED`.
- `FAULT_DETECTED` issued internally → transitions to `FAULT`.
- `ABORT_CMD` issued by an operator → transitions to `DISARMING`.

---

### `ENABLED`

Reached when an operator issues `ENABLE_CMD`, transitioning from `ARMED` to 
`ENABLED`. At this point the operator can issue one final command to reach the 
terminal state and deploy the device.

**Possible transitions from this state:**

- `FIRE_CMD` issued by an operator → transitions to `FIRED`.
- `FAULT_DETECTED` issued internally → transitions to `FAULT`.
- `ABORT_CMD` issued by an operator → transitions to `DISARMING`.

---

### `FIRED` — Terminal State

The terminal state. Reached when `FIRE_CMD` is issued whilst in the `ENABLED` 
state, firing the missile/torpedo. No further transitions are possible from 
this state.

---

### `FAULT`

If any hardware or software fault is detected at any stage — including but not 
limited to `ARMING`, `ARMED`, `ENABLED`, and `DISARMING` — the system 
transitions to `FAULT`. Safety is always the priority; no further operational 
transitions can occur until the fault is resolved.

This state is also valuable for diagnostics. Logs can be examined to identify 
exactly when and from which state the system transitioned to `FAULT`, helping 
engineers spot and diagnose errors more quickly.

**Possible transitions from this state:**

- `RESET_CMD` issued by an operator → transitions to `DISARMING`.

---

### `DISARMING`

Entered when an `ABORT_CMD` is issued from `ARMING`, `ARMED`, or `ENABLED`, or 
when a `RESET_CMD` is issued from `FAULT`. The system works backwards through 
the same process described in `ARMING`, ensuring all safety mechanisms are 
restored and active before returning to `SAFE`.

Think of `RESET_CMD` as the embedded equivalent of turning it off and on again — 
except with considerably higher stakes.

**Possible transitions from this state:**

- `DISARMING_COMPLETE` issued internally → transitions to `SAFE`.
- `FAULT_DETECTED` issued internally → transitions to `FAULT`.

---

## Commands

| Command | Type | Description |
|---------|------|-------------|
| `ARM_CMD` | Operator | Initiates the arming sequence from `SAFE` |
| `ABORT_CMD` | Operator | Aborts and initiates disarming from `ARMING`, `ARMED`, or `ENABLED` |
| `ENABLE_CMD` | Operator | Enables firing from `ARMED` |
| `FIRE_CMD` | Operator | Fires the device from `ENABLED` |
| `RESET_CMD` | Operator | Initiates disarming sequence from `FAULT` |
| `ARMING_COMPLETE` | Internal | Confirms all safety mechanisms removed |
| `DISARMING_COMPLETE` | Internal | Confirms all safety mechanisms restored |
| `FAULT_DETECTED` | Internal | Signals a hardware or software fault |


