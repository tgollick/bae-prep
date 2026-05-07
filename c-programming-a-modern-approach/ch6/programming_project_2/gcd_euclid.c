#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Defining the two variables used in the Euclid Algorithm
    // Defining variables rather than declaring to follow best practices mentioned in previous book Modern C
    int m = 0;
    int n = 0;

    // Taking input of two values
    // Could be safer ways of taking input but that isn't the focus right now
    printf("Enter two integers:");
    scanf("%d %d", &m, &n);

    // Check if values are below 0 and convert to non-negative using absolute function to handle negative values
    if (m < 0)
        m = abs(m);
    if (n < 0)
        m = abs(n);

    // While loop is most appropriate since we continue until a specific condition is met and don't
    // require any form of indexing
    while (n != 0)
    {
        // Euclid Algorithm
        // Compute remainder of m / n
        // m = n
        // n = remainder
        // Repeat until n = 0
        int remainder = m % n;
        m = n;
        n = remainder;
    }

    // Produce output
    printf("Greatest common divisor: %d", m);

    return 0;
}