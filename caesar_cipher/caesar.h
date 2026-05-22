#ifndef CAESAR_H
#define CAESAR_H

#define SHIFT 3

char encrypt_char(char ch);
char decrypt_char(char ch);
void process_file(const char *input_path, const char *output_path,
                  char (*transform)(char));

#endif
