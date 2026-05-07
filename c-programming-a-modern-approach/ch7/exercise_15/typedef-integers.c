#include <stdio.h>

int main()
{
    typedef char Int8;
    typedef short Int16;
    typedef int Int32;

    Int8 number1 = 12;
    Int16 number2 = 67;
    Int32 number3 = 120;

    printf("Size of Int8: %zu\n", sizeof(number1));
    printf("Size of Int16: %zu\n", sizeof(number2));
    printf("Size of Int32: %zu\n", sizeof(number3));

    return 0;
}