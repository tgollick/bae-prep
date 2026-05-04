# BAE Prep Repository Setup Script
# Run this from inside your locally cloned bae-prep folder
# Command: powershell -ExecutionPolicy Bypass -File setup_bae_prep.ps1

# ── Helper ──────────────────────────────────────────────────────────────
function New-File {
    param([string]$Path, [string]$Content)
    $dir = Split-Path $Path -Parent
    if ($dir -and -not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }
    Set-Content -Path $Path -Value $Content -Encoding UTF8
}

# ── ROOT ────────────────────────────────────────────────────────────────
New-File "README.md" @"
# BAE Systems — Pre-Employment Prep

Software Engineering preparation for BAE Maritime Services (Spearfish programme), September 2025 start.

## Structure

| Folder | Purpose |
|--------|---------|
| ``modern-c/`` | Exercises from Modern C by Jens Gustedt |
| ``c-state-machine/`` | Phase I project — embedded C FSM |
| ``ada-state-machine/`` | Phase II project — Ada 2012 + SPARK port |
| ``torpedo-trajectory-sim/`` | Phase III project — Python numerical simulation |
| ``notes/`` | Cross-cutting notes and Anki exports |

## Timeline

- **May–June:** Phase I — C and embedded systems
- **July:** Phase II — Ada and SPARK formal verification
- **August:** Phase III — Python numerical modelling
"@

New-File ".gitignore" @"
# C build artefacts
*.o
*.out
*.exe
a.out

# Logs
logs/

# Python
__pycache__/
*.pyc
*.pyo
.venv/
venv/
*.egg-info/

# Jupyter
.ipynb_checkpoints/

# Plot outputs
outputs/

# Ada
*.ali
*.o
obj/
*.gpr~

# OS
.DS_Store
Thumbs.db

# Editor
.vscode/
*.swp
"@

# ── MODERN C ────────────────────────────────────────────────────────────
New-File "modern-c/README.md" "# Modern C Exercises`nExercises from Modern C by Jens Gustedt, worked through chapter by chapter."

# Level 0 — Ch 1–5
foreach ($ch in @("ch01","ch02","ch03","ch04","ch05")) {
    New-File "modern-c/level-0/$ch/exercises.c" "/* Modern C — Level 0 — $ch exercises */"
    New-File "modern-c/level-0/$ch/notes.md"    "# $ch Notes"
}

# Level 1 — Ch 6–15
foreach ($ch in @("ch06","ch07","ch08","ch09","ch10","ch11","ch12","ch13","ch14","ch15")) {
    New-File "modern-c/level-1/$ch/exercises.c" "/* Modern C — Level 1 — $ch exercises */"
    New-File "modern-c/level-1/$ch/notes.md"    "# $ch Notes"
}

# Level 2 + scratch
New-File "modern-c/level-2/.gitkeep" ""
New-File "modern-c/scratch/.gitkeep" ""

# ── C STATE MACHINE ─────────────────────────────────────────────────────
New-File "c-state-machine/README.md"               "# C State Machine`nPhase I project — arming/disarming sequence modelled as an embedded C finite state machine."
New-File "c-state-machine/Makefile"                "# Makefile — build instructions to be added"
New-File "c-state-machine/design/state-diagram.md" "# State Diagram`nDocument your state diagram design here before writing any code."
New-File "c-state-machine/design/decisions.md"     "# Design Decisions`nRecord why you made specific design choices here."
New-File "c-state-machine/include/state_machine.h" "/* state_machine.h — State and Event enum definitions, StateMachine struct */"
New-File "c-state-machine/include/events.h"        "/* events.h — Event definitions (merge into state_machine.h if preferred) */"
New-File "c-state-machine/src/main.c"              "/* main.c — Entry point, feeds events into the state machine */"
New-File "c-state-machine/src/state_machine.c"     "/* state_machine.c — Dispatch logic and transition table */"
New-File "c-state-machine/src/actions.c"           "/* actions.c — Entry and exit actions for each state */"
New-File "c-state-machine/src/logger.c"            "/* logger.c — File logging module */"
New-File "c-state-machine/tests/test_transitions.c" "/* test_transitions.c — Assert-based unit tests for state transitions */"
New-File "c-state-machine/tests/test_helpers.h"    "/* test_helpers.h — Shared test utility functions */"
New-File "c-state-machine/logs/.gitkeep"           ""

# ── ADA STATE MACHINE ───────────────────────────────────────────────────
New-File "ada-state-machine/README.md"                    "# Ada State Machine`nPhase II project — port of the C state machine into Ada 2012 with SPARK formal verification contracts."
New-File "ada-state-machine/ada_state_machine.gpr"        "-- Ada project file — to be configured when Alire is set up"
New-File "ada-state-machine/design/spark-contracts.md"    "# SPARK Contracts`nDocument Pre/Post condition decisions before writing any Ada code."
New-File "ada-state-machine/src/state_machine.ads"        "-- state_machine.ads — Package specification: types, contracts"
New-File "ada-state-machine/src/state_machine.adb"        "-- state_machine.adb — Package body: implementation"
New-File "ada-state-machine/src/actions.ads"              "-- actions.ads — Actions package specification"
New-File "ada-state-machine/src/actions.adb"              "-- actions.adb — Actions package body"
New-File "ada-state-machine/src/logger.ads"               "-- logger.ads — Logger package specification"
New-File "ada-state-machine/src/logger.adb"               "-- logger.adb — Logger package body"
New-File "ada-state-machine/src/main.adb"                 "-- main.adb — Entry point"
New-File "ada-state-machine/tests/.gitkeep"               ""

# ── TORPEDO TRAJECTORY SIM ──────────────────────────────────────────────
New-File "torpedo-trajectory-sim/README.md"                    "# Torpedo Trajectory Simulation`nPhase III project — numerical simulation of underwater torpedo trajectory using SciPy ODE solvers."
New-File "torpedo-trajectory-sim/requirements.txt"             "numpy`nscipy`nmatplotlib"
New-File "torpedo-trajectory-sim/design/physics-model.md"      "# Physics Model`nDocument the drag equations, buoyancy model, and guidance law before writing any code."
New-File "torpedo-trajectory-sim/src/main.py"                  "# main.py — Entry point, runs simulations"
New-File "torpedo-trajectory-sim/src/physics.py"               "# physics.py — Drag model, buoyancy, equations of motion"
New-File "torpedo-trajectory-sim/src/integrators.py"           "# integrators.py — Euler and RK4 integrators written from scratch"
New-File "torpedo-trajectory-sim/src/guidance.py"              "# guidance.py — Proportional navigation guidance law"
New-File "torpedo-trajectory-sim/src/visualisation.py"         "# visualisation.py — All Matplotlib plotting"
New-File "torpedo-trajectory-sim/notebooks/exploration.ipynb"  '{"cells":[],"metadata":{"kernelspec":{"display_name":"Python 3","language":"python","name":"python3"}},"nbformat":4,"nbformat_minor":4}'
New-File "torpedo-trajectory-sim/tests/test_integrators.py"    "# test_integrators.py — Validate Euler and RK4 against known analytical solutions"
New-File "torpedo-trajectory-sim/outputs/.gitkeep"             ""

# ── NOTES ───────────────────────────────────────────────────────────────
New-File "notes/rtos-notes.md"      "# RTOS Notes`nNotes from Mastering the FreeRTOS Kernel — Weeks 7-9."
New-File "notes/maths-notes.md"     "# Maths Notes`n3Blue1Brown Linear Algebra and Calculus series notes."
New-File "notes/resources.md"       "# Resources`nAnnotated resource list — add personal notes on each resource as you use it."
New-File "notes/anki-export/.gitkeep" ""

# ── DONE ────────────────────────────────────────────────────────────────
Write-Host ""
Write-Host "Structure created. Now run:" -ForegroundColor Green
Write-Host "  git add ." -ForegroundColor Cyan
Write-Host "  git commit -m 'chore: initialise repo structure'" -ForegroundColor Cyan
Write-Host "  git push" -ForegroundColor Cyan
