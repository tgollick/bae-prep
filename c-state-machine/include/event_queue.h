#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <stdbool.h>

#define QUEUE_SIZE 16

typedef int Event;

typedef struct {
  Event buffer[QUEUE_SIZE];
  int head;
  int tail;
  int count;
} EventQueue;

void queue_init(void);
bool enqueue(Event event);
bool dequeue(Event *output);
bool is_full(void);
bool is_empty(void);

#endif
