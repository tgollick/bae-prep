#include <stdio.h>

int main() {
    
    // Integer overflow

    // Predicition: This I would guess from my reading so far may either produce a similar,
    // wrap around modulo effect as size_t or will simply overflow and cause the program to crash
    
    // Actual result: C doesn't treat an integer overflow as a crash but as undefined behaviour
    // the compiler will define the outcome/behaviour which in this case was a wrap around to the,
    // largest negative value a short can be which is -32767, you cannot rely on this wrap around,
    // as different compilers will have different outcomes, best to confirm values before the
    // operation to be safe
    short v1 = 32767;
    v1 += 1;

    printf("Overflow result: %d\n\n", v1);

    // Signed vs Unsigned mismatch

    // Predicition: I believe that this will default to false, but honestly I am not sure on
    // the expected behaviour

    // Actual result: When you compare a signed to an unsigned value C's rules clearly state
    // that you convert the signed value to unsigned, in this case -1 becomes the
    // largest value possible as -1 in binary is all bits set to 1, therefore the comparison is
    // deemed true, this is called implicit conversion and is one of the most dangrous mistakes,
    // you can make in embedded programming.
    signed int v2 = -1;
    unsigned int v3 = 1;

    printf("-1 is greater than 1?: %d\n\n", (v2 > v3));

    // Size of comparison

    // Prediciton: Some of these I have no clue, I do believe:
    // Char should be 8 bytes
    // Short I would say is 2 bytes
    // Int is 4 bytes (32 bits, although system specific)
    // Long I would say is 8 bytes
    // Double I would guess is 8 bytes also

    // Actual result: Pretty much all true, biggest incorrect answer was char where its always,
    // 1 byte, otherwise all correct other than long which actually differs in size depending on platform
    // for example on mac its 8 bytes and windows its 4 bytes which emphasises the importance in
    // using fixed width data types provided by the C library when programming for specific memory,
    // constraints where assumptions cannot be made.
    printf("Size of char is %zu bytes\n", sizeof(char));
    printf("Size of short is %zu bytes\n", sizeof(short));
    printf("Size of int is %zu bytes\n", sizeof(int));
    printf("Size of long is %zu bytes\n", sizeof(long));
    printf("Size of double is %zu bytes\n", sizeof(double));
    
    return 0;
}