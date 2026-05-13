#include <stdbool.h>
#include <stdio.h>

int main()
{
    int seen_digits[10] = {0};
    bool repeated = false;
    int digit;
    long n;

    printf("Enter a number: ");
    scanf("%ld", &n);

    while (n > 0)
    {
        digit = n % 10;

        if (seen_digits[digit] == 1)
        {
            repeated = true;
        }

        seen_digits[digit]++;
        n /= 10;
    }

    if (repeated)
    {
        printf("Repeated digits: ");

        for (int i = 0; i < 10; i++)
        {
            if (seen_digits[i] >= 2)
            {
                printf("%d ", i);
            }
        }
    }
    else
    {
        printf("No repeated digit!");
    }

    return 0;
}