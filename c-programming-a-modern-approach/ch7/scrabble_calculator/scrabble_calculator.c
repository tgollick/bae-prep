#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Creating an optimised version which uses the char - 'A' to detmine its lookup index in the array points
size_t optimised_count(char word[]) {
    // Total points for the word fed in
    size_t word_points = 0;

    // Defining all the points for each character a - z
    static const int points[26] = {
        1,  // A
        3,  // B
        3,  // C
        2,  // D
        1,  // E
        4,  // F
        2,  // G
        4,  // H
        1,  // I
        8,  // J
        5,  // K
        1,  // L
        3,  // M
        1,  // N
        1,  // O
        3,  // P
        10, // Q
        1,  // R
        1,  // S
        1,  // T
        1,  // U
        4,  // V
        4,  // W
        8,  // X
        4,  // Y
        10  // Z
    };

    // Looping through, casting the character in word to an unsigned char for compatability reasons relating to toupper()
    for(size_t i = 0; word[i] != '\0'; i++) {
        char c = toupper((unsigned char)word[i]);

        // If not a valid character ignore otherwise add points from the index of the points array by c - 'A'
        if(c >= 'A' && c <= 'Z') {
            word_points += points[c - 'A'];
        }
    }

    return word_points;
}

int main() {
    size_t word_points = 0;

    printf("Please enter your word: ");
    
    // Read word from user
    char str[50];
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';

    // Go through each character to work out individual points, converting to upper case for continuity
    // Using the new line as a marker for the end of the word rather than the null terminator to stop it
    // being processed in the switch statement
    for(size_t i = 0; str[i] != '\0' ; i++) {
        switch(toupper(str[i])) {
            case 'Q':
            case 'Z':
                word_points += 10;
                break;
            case 'J':
            case 'X':
                word_points += 8;
                break;
            case 'K':
                word_points += 5;
                break;
            case 'F':
            case 'H':
            case 'V':
            case 'W':
            case 'Y':
                word_points += 4;
                break;
            case 'B':
            case 'C':
            case 'M':
            case 'P':
                word_points += 3;
                break;
            case 'D':
            case 'G':
                word_points += 2;
                break;
            case 'A':
            case 'E':
            case 'I':
            case 'L':
            case 'N':
            case 'O':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
                word_points += 1;
                break;
            default:
                break;
        }
    }
    
    // Print final outcome and word
    printf("Total points for the word %s is %zu\n", str, word_points);

    // Printing output for optmised function also
    printf("OPTIMISED: Total points for the word %s is %zu\n", str, optimised_count(str));

    return 0;
}

