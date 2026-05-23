#include "caesar.h"
#include <stdlib.h>

int main(void) {
  // Handling an encryption of input.
  process_file("input.txt", "encrypted.txt", encrypt_char);

  // Handling a decryption of the just created encrypted file
  process_file("encrypted.txt", "decrypted.txt", decrypt_char);

  exit(EXIT_SUCCESS);
}
