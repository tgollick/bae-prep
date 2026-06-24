#include "clock.h"
#include "event_queue.h"
#include "events.h"
#include "interlock.h"
#include "router.h"
#include "state_machine.h"
#include "watchdog.h"
#include <stdint.h>
#include <stdio.h>

static int checks_run = 0;
static int checks_failed = 0;

#define CHECK(cond)                                                            \
  do {                                                                         \
    checks_run++;                                                              \
    if (!(cond)) {                                                             \
      checks_failed++;                                                         \
      printf("FAIL [%s:%d] %s\n", __FILE__, __LINE__, #cond);                  \
    }                                                                          \
  } while (0)

static void test_queue(void) {
  // Init queue and check shows as empty
  queue_init();
  CHECK(is_empty());

  // Enqueue an item and check that queue is no longer empty
  enqueue(CMD_ARM);
  CHECK(!is_empty());

  // Check dequeued item is correct
  Event temp;
  dequeue(&temp);
  CHECK(temp == CMD_ARM);

  // Check queue is no empty again
  CHECK(is_empty());

  // Check that dequeue fails when empty
  CHECK(!dequeue(&temp));

  // Loop adding a ARM command until full
  for (int i = 0; i < 16; i++) {
    enqueue(CMD_ARM);
  }

  // Check queue is now full
  CHECK(is_full());

  // Check that you cannot add to full queue
  CHECK(!enqueue(CMD_ARM));
}

static void test_watchdog_check(void) {
  clock_init();
  Watchdog wd = {.enabled = false, .start_tick = now(), .limit = 5};

  // if disabled: always false
  CHECK(!check_time(&wd));

  wd.enabled = true;

  // arm at current tick
  wd.start_tick = now();

  // elapsed 0, under limit
  CHECK(!check_time(&wd));

  for (int i = 0; i < 4; i++)
    clock_tick();

  // elapsed 4, still under limit of 5
  CHECK(!check_time(&wd));
  clock_tick();

  // elapsed 5, at limit -> true (it's >=)
  CHECK(check_time(&wd));
}

/* ---- Transition tests --------------------------------------------------- */
/* Force a known source state, dispatch one command, check the resulting      */
/* state against the value hard-coded here. */
static void check_main(State from, Command cmd, State expected) {
  StateMachine sm = {.current_state = from}; /* wd zero-inits */
  dispatch(&sm, cmd);
  checks_run++;
  if (sm.current_state != expected) {
    checks_failed++;
    printf("FAIL: main [%s] + %s -> expected %s, got %s\n",
           state_to_string(from), command_to_string(cmd),
           state_to_string(expected), state_to_string(sm.current_state));
  }
}

static void check_interlock(InterlockState from, Command cmd,
                            InterlockState expected) {
  InterlockStateMachine ism = {.current_state = from};
  interlock_dispatch(&ism, cmd);
  checks_run++;
  if (ism.current_state != expected) {
    checks_failed++;
    printf("FAIL: interlock [%s] + %s -> expected %s, got %s\n",
           interlock_state_to_string(from), command_to_string(cmd),
           interlock_state_to_string(expected),
           interlock_state_to_string(ism.current_state));
  }
}

static void test_main_transitions(void) {
  /* --- Valid operational path --- */
  check_main(STATE_SAFE, CMD_ARM, STATE_ARMING);
  check_main(STATE_ARMING, CMD_INTERLOCK_OPENED, STATE_ARMED);
  check_main(STATE_ARMED, CMD_ENABLE, STATE_ENABLED);
  check_main(STATE_ENABLED, CMD_FIRE, STATE_FIRED);
  check_main(STATE_ARMING, CMD_ABORT, STATE_DISARMING);
  check_main(STATE_ARMED, CMD_ABORT, STATE_DISARMING);
  check_main(STATE_ENABLED, CMD_ABORT, STATE_DISARMING);
  check_main(STATE_DISARMING, CMD_INTERLOCK_CLOSED, STATE_SAFE);
  check_main(STATE_FAULT, CMD_RESET, STATE_DISARMING);

  /* --- Fault entry from every operational state --- */
  check_main(STATE_SAFE, CMD_FAULT_DETECTED, STATE_FAULT);
  check_main(STATE_SAFE, CMD_INTERLOCK_FAULT_DETECTED, STATE_FAULT);
  check_main(STATE_ARMING, CMD_FAULT_DETECTED, STATE_FAULT);
  check_main(STATE_ARMING, CMD_INTERLOCK_FAULT_DETECTED, STATE_FAULT);
  check_main(STATE_ARMED, CMD_FAULT_DETECTED, STATE_FAULT);
  check_main(STATE_ARMED, CMD_INTERLOCK_FAULT_DETECTED, STATE_FAULT);
  check_main(STATE_ENABLED, CMD_FAULT_DETECTED, STATE_FAULT);
  check_main(STATE_ENABLED, CMD_INTERLOCK_FAULT_DETECTED, STATE_FAULT);
  check_main(STATE_DISARMING, CMD_FAULT_DETECTED, STATE_FAULT);
  check_main(STATE_DISARMING, CMD_INTERLOCK_FAULT_DETECTED, STATE_FAULT);

  /* --- Safety-critical rejections: state must NOT change --- */
  /* Dual-interlock guarantee: ARMED is reachable ONLY via ARMING +         */
  /* INTERLOCK_OPENED. A direct SAFE->ARMED shortcut must be impossible.     */
  check_main(STATE_SAFE, CMD_INTERLOCK_OPENED, STATE_SAFE);
  check_main(STATE_SAFE, CMD_ENABLE, STATE_SAFE);
  check_main(STATE_SAFE, CMD_FIRE, STATE_SAFE);
  /* No premature firing: ARMED cannot jump straight to FIRED. */
  check_main(STATE_ARMED, CMD_FIRE, STATE_ARMED);
  /* No re-arming or step-skipping. */
  check_main(STATE_ARMING, CMD_ARM, STATE_ARMING);
  check_main(STATE_ARMING, CMD_ENABLE, STATE_ARMING);
  check_main(STATE_ARMED, CMD_ARM, STATE_ARMED);
  check_main(STATE_ENABLED, CMD_ENABLE, STATE_ENABLED);
  /* FIRED is terminal: nothing moves it, not even a fault. */
  check_main(STATE_FIRED, CMD_ABORT, STATE_FIRED);
  check_main(STATE_FIRED, CMD_FAULT_DETECTED, STATE_FIRED);
  /* FAULT leaves only via RESET. */
  check_main(STATE_FAULT, CMD_ARM, STATE_FAULT);
  check_main(STATE_FAULT, CMD_ABORT, STATE_FAULT);
  check_main(STATE_FAULT, CMD_FAULT_DETECTED, STATE_FAULT);
}

static void test_interlock_transitions(void) {
  /* --- Valid operational path --- */
  check_interlock(STATE_CLOSED, CMD_ARM, STATE_OPENING);
  check_interlock(STATE_OPENING, CMD_INTERLOCK_OPENED, STATE_OPEN);
  check_interlock(STATE_OPENING, CMD_ABORT, STATE_CLOSING);
  check_interlock(STATE_OPEN, CMD_ABORT, STATE_CLOSING);
  check_interlock(STATE_CLOSING, CMD_INTERLOCK_CLOSED, STATE_CLOSED);
  check_interlock(STATE_FAULT_INTERLOCK, CMD_RESET, STATE_CLOSING);

  /* --- Fault entry --- */
  check_interlock(STATE_CLOSED, CMD_FAULT_DETECTED, STATE_FAULT_INTERLOCK);
  check_interlock(STATE_CLOSED, CMD_INTERLOCK_FAULT_DETECTED,
                  STATE_FAULT_INTERLOCK);
  check_interlock(STATE_OPEN, CMD_FAULT_DETECTED, STATE_FAULT_INTERLOCK);
  check_interlock(STATE_OPEN, CMD_INTERLOCK_FAULT_DETECTED,
                  STATE_FAULT_INTERLOCK);
  check_interlock(STATE_OPENING, CMD_FAULT_DETECTED, STATE_FAULT_INTERLOCK);
  check_interlock(STATE_OPENING, CMD_INTERLOCK_FAULT_DETECTED,
                  STATE_FAULT_INTERLOCK);
  check_interlock(STATE_CLOSING, CMD_FAULT_DETECTED, STATE_FAULT_INTERLOCK);
  check_interlock(STATE_CLOSING, CMD_INTERLOCK_FAULT_DETECTED,
                  STATE_FAULT_INTERLOCK);

  /* --- Rejections: state must NOT change --- */
  check_interlock(STATE_CLOSED, CMD_INTERLOCK_OPENED, STATE_CLOSED);
  check_interlock(STATE_CLOSED, CMD_ABORT, STATE_CLOSED);
  check_interlock(STATE_OPEN, CMD_ARM, STATE_OPEN);
  check_interlock(STATE_OPENING, CMD_ARM, STATE_OPENING);
  check_interlock(STATE_OPENING, CMD_INTERLOCK_CLOSED, STATE_OPENING);
  check_interlock(STATE_CLOSING, CMD_INTERLOCK_OPENED, STATE_CLOSING);
  check_interlock(STATE_CLOSING, CMD_ABORT, STATE_CLOSING);
  check_interlock(STATE_FAULT_INTERLOCK, CMD_ARM, STATE_FAULT_INTERLOCK);
  check_interlock(STATE_FAULT_INTERLOCK, CMD_FAULT_DETECTED,
                  STATE_FAULT_INTERLOCK);
}

static void test_transitions(void) {
  /* Run both tests for main and interlock state machine */
  test_main_transitions();
  test_interlock_transitions();
}
static void test_watchdog_arming(void) {
  StateMachine sm = {.current_state = STATE_SAFE};
  InterlockStateMachine ism = {.current_state = STATE_CLOSED};

  router(CMD_ARM, &sm, dispatch, &ism, interlock_dispatch);

  // Check that after consuming CMD_ARM they are now watchdog enabled
  CHECK(sm.wd.enabled);
  CHECK(ism.wd.enabled);

  // And that their limits have been correctly set based off the timeout tables
  CHECK(sm.wd.limit == 800);
  CHECK(ism.wd.limit == 500);
}
static void test_fault_recovery(void) {
  StateMachine sm = {.current_state = STATE_ARMED};
  InterlockStateMachine ism = {.current_state = STATE_OPEN};

  router(CMD_FAULT_DETECTED, &sm, dispatch, &ism, interlock_dispatch);

  CHECK(sm.current_state == STATE_FAULT);
  CHECK(ism.current_state == STATE_FAULT_INTERLOCK);

  router(CMD_RESET, &sm, dispatch, &ism, interlock_dispatch);

  CHECK(sm.current_state == STATE_DISARMING);
  CHECK(ism.current_state == STATE_CLOSING);
}

int main(void) {
  test_queue();
  test_watchdog_check();
  test_transitions();
  test_watchdog_arming();
  test_fault_recovery();

  printf("\n%d checks, %d failed\n", checks_run, checks_failed);
  return checks_failed > 0 ? 1 : 0;
}
