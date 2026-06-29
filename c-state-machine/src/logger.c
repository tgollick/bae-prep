#include "logger.h"

static Logger l;

void log_init(char *op) {
  l.count = 0;
  l.head = 0;
  l.tail = 0;
  l.output_path = op;
}

bool log_add(Log log) {
  // If log is full overwrite oldest log
  if (log_is_full()) {
    // Overwrite oldest log

    // Take current log and place at head (oldest log)
    l.buffer[l.head] = log;

    // Apply same wrap around functionality for head
    l.head = (l.head + 1) % LOG_SIZE;

    return true;
  }
  // Otherwise add new value to the buffer at tail position
  else {
    l.buffer[l.tail] = log;

    // Increase count to reflect new added event
    l.count++;

    // Handle the new tail value and wrap around logic in one clean line
    l.tail = (l.tail + 1) % LOG_SIZE;

    // Return true to indicate success
    return true;
  }
}

bool log_flush(void) {
  while (!log_is_full()) {
    // Drain the queue and output logs to output_path in csv format?
  }

  return true;
}

bool log_is_empty() { return l.count == 0; }

bool log_is_full() { return l.count == LOG_SIZE; }
