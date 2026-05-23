#define _CRT_SECURE_NO_WARNINGS

#include "caesar.h"
#include <stdio.h>

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
                  char (*transform)(char)) {

  // Opening the input file
  FILE *in = fopen(input_path, "r");

  // Checking the file is valid otherwise "in" is a NULL pointer, return if so
  if (in == NULL) {
    fprintf(stderr, "Error: could not open %s\n", input_path);
    return;
  }

  // Opening the text file to produce the output to
  FILE *out = fopen(output_path, "w");

  // Again checking if the output file exists and is valid, otherwise same error
  // of a NULL pointer before, return if so
  if (out == NULL) {
    fprintf(stderr, "Error: could not open %s\n", output_path);
  }

  // Define a buffer of 256 length to hold each line of the input file
  char buffer[256];

  while (fgets(buffer, sizeof(buffer), in) != NULL) {
    // At this point buffer contains onle line including the newline char
    for (char *p = buffer; *p != '\0'; p++) {
      // Transforming each character based on the function passed in (encrypt or
      // decrypt)
      fputc(transform(*p), out);
    }
  }

  // Closing the input output file streams before ending the function
  fclose(in);
  fclose(out);
}
