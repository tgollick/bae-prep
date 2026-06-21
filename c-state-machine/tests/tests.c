#include "event_queue.h"
#include "events.h"
#include "interlock.h"
#include "state_machine.h"
#include "watchdog.h"
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
  Watchdog wd;

  // Set enabled to false and check with check_time function
  wd.enabled = false;
  CHECK(!check_time(&wd));

  // Don't really know what else to test for here...
}
static void test_transitions(void) { /* */ }
static void test_watchdog_arming(void) { /* ... */ }
static void test_fault_recovery(void) { /* ... */ }

int main(void) {
  test_queue();
  test_watchdog_check();
  test_transitions();
  test_watchdog_arming();
  test_fault_recovery();

  printf("\n%d checks, %d failed\n", checks_run, checks_failed);
  return checks_failed > 0 ? 1 : 0;
}
