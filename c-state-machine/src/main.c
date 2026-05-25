#include "event_queue.h"
#include "events.h"
#include "interlock.h"
#include "router.h"
#include "state_machine.h"
#include <Windows.h>
#include <stdio.h>

int main(void) {

  // Init the event queue to hold upcoming events
  queue_init();

  // Init the two state machines
  StateMachine sm = {.current_state = STATE_SAFE};
  InterlockStateMachine ism = {.current_state = STATE_CLOSED};

  // Enqueue events to test the event queue, routing and dispatch functions
  enqueue(CMD_ARM); // StateMachine: SAFE -> ARMING / InterlockStateMachine:
                    // CLOSED -> OPENING
  enqueue(CMD_INTERLOCK_OPENED); // StateMachine: ARMING -> ARMED /
                                 // InterlockStateMachine: OPENING -> OPEN
  enqueue(CMD_ABORT);            // StateMachine: ARMED -> DISARMING /
                                 // InterlockStateMachine: OPEN -> CLOSING
  enqueue(CMD_INTERLOCK_CLOSED); // StateMachine: DISARMING -> SAFE /
                                 // InterlockStateMachine: CLOSING -> CLOSED

  // Event enum to hold next command
  Event e;

  // Drain the queue using a loop and dequeue
  while (dequeue(&e)) {
    // Use the router to pass the command to the required state machine and
    // transition state when valid
    router(e, &sm, dispatch, &ism, interlock_dispatch);
  }

  printf("\nPROGRAM COMPLETE!");

  return 0;
}
