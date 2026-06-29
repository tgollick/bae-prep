#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>

#define LOG_SIZE 16

typedef int Log;

typedef struct {
  Log buffer[LOG_SIZE];
  int head;
  int tail;
  int count;
  char *output_path;
} Logger;

void log_init(char *op);
bool log_add(Log l);
bool log_flush(void);
bool log_is_full(void);
bool log_is_empty(void);

#endif
