#include "event_queue.h"
#include "state_machine.h"
#include <stdio.h>

int main(void) {
  int next_cmd;

  queue_init();

  enqueue(CMD_ARM);
  enqueue(CMD_ARMING_COMPLETE);
  enqueue(CMD_FIRE);

  printf("Queue is empty: %s\n", is_empty() ? "True" : "False");

  while (!is_empty()) {
    dequeue(&next_cmd);
    printf("Command dequeued: %s\n", state_to_string(next_cmd));
  }

  printf("Queue is empty: %s\n", is_empty() ? "True" : "False");

  return 1;
}
