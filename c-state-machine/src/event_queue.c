#include "event_queue.h"

EventQueue q;

void queue_init() {
  q.count = 0;
  q.head = 0;
  q.tail = 0;
}

bool enqueue(Event event) {
  // If buffer is full return false to reflect fail
  if (is_full()) {
    return false;
  }
  // Otherwise add new value to the buffer at tail position
  else {
    q.buffer[q.tail] = event;

    // Increase count to reflect new added event
    q.count++;

    // Handle the new tail value and wrap around logic in one clean line
    q.tail = (q.tail + 1) % QUEUE_SIZE;

    // Return true to indicate success
    return true;
  }
}

bool dequeue(Event *output) {
  // The ensure we can indicate an empty event buffer than dequeues we return
  // bool instead and pass in the output location for the event if valid
  // dequeues
  if (is_empty()) {
    return false;
  }
  // Else return the next up event indicated by head
  else {
    // Return the current next event to the output pointer
    *output = q.buffer[q.head];

    // Then return true to indicate a valid dequeue
    return true;
  }
}

bool is_empty() { return q.count == 0; }

bool is_full() { return q.count == QUEUE_SIZE; }
