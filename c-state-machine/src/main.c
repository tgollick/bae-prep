#include "clock.h"
#include "event_queue.h"
#include "events.h"
#include "interlock.h"
#include "router.h"
#include "state_machine.h"
#include "watchdog.h"
#include <stdio.h>

int main(void) {
  // Init both the clock and queue
  clock_init();
  queue_init();

  // Init the two state machines
  StateMachine sm = {
      .current_state = STATE_SAFE, .wd.enabled = false, .wd.limit = 3000};
  InterlockStateMachine ism = {
      .current_state = STATE_CLOSED, .wd.enabled = false, .wd.limit = 3000};

  // Enqueue events to test the event queue, routing and dispatch functions
  enqueue(CMD_ARM); // StateMachine: SAFE -> ARMING / InterlockStateMachine:
                    // CLOSED -> OPENING
  enqueue(CMD_INTERLOCK_OPENED); // StateMachine: ARMING -> ARMED /
                                 // InterlockStateMachine: OPENING -> OPEN
  enqueue(CMD_ABORT);            // StateMachine: ARMED -> DISARMING /
                                 // InterlockStateMachine: OPEN -> CLOSING
  enqueue(CMD_INTERLOCK_CLOSED); // StateMachine: DISARMING -> SAFE /
                                 // InterlockStateMachine: CLOSING -> CLOSED

  while (sm.current_state != STATE_FAULT ||
         ism.current_state != STATE_FAULT_INTERLOCK) {

    // Update global timer tick
    clock_tick();

    // Event enum to hold next command
    Event e;

    // Drain the queue using a loop and dequeue
    while (dequeue(&e)) {
      // Use the router to pass the command to the required state machine and
      // transition state when valid
      router(e, &sm, dispatch, &ism, interlock_dispatch);

      printf("\n");
    }

    // Creating a pretend "stall", checked and works fine as expected
    sm.wd.enabled = true;
    sm.wd.start_tick = now() - 3500;

    // Check timer of current state against global for timeouts
    if (check_time(&sm.wd) || check_time(&ism.wd)) {
      printf("LOG: Timeout detected, stalling! FAULT_DETECTED CMD EXECUTED!\n");
      enqueue(CMD_FAULT_DETECTED);
    }
  }

  printf("PROGRAM EXECUTION COMPLETE!");

  return 0;
}
