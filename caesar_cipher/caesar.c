#include "caesar.h"

char encrypt_char(char ch) {
  // Handle cases
  if (ch >= 65 && ch <= 90) {
    // Upper case
    return ((ch - 'A') + SHIFT) % 26 + 'A';
  } else if (ch >= 97 && ch <= 122) {
    // Lower case
    return ((ch - 'a') + SHIFT) % 26 + 'a';
  }

  // Otherwise ignore and return the original value
  return ch;
}

char decrypt_char(char ch) {
  // Handle cases
  if (ch >= 65 && ch <= 90) {
    // Upper case
    return ((ch - 'A') - SHIFT + 26) % 26 + 'A';
  } else if (ch >= 97 && ch <= 122) {
    // Lower case
    return ((ch - 'a') - SHIFT + 26) % 26 + 'a';
  }

  // Otherwise ignore and return the original value
  return ch;
}

void process_file(const char *input_path, const char *output_path,
                  char (*transform)(char)) {}
