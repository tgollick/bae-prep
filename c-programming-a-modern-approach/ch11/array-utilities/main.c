#include "array_utils.h"
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 6

int main(void)
{
    int arr[6] = {2, 6, 9, 10, 4, 21};

    printf("Starting values: ");
    print_arr(arr, ARRAY_SIZE);

    // Testing the swap function
    swap(&arr[1], &arr[4]);
    printf("\nValues after swap function: ");
    print_arr(arr, ARRAY_SIZE);

    // Testing the min and max functions
    printf("\nMax Value is: %d\n", *max_element(arr, ARRAY_SIZE));
    printf("Min Value is: %d\n", *min_element(arr, ARRAY_SIZE));

    // Testing the reverse in place function
    reverse_in_place(arr, ARRAY_SIZE);
    printf("\nValues after reverse in place function: ");
    print_arr(arr, ARRAY_SIZE);

    printf("\n");

    // Testing the linear search function
    int *res = linear_search(arr, ARRAY_SIZE, 10);
    if (res == NULL)
    {
        printf("Target 10, not found in array...");
    }
    else
    {
        printf("Target 10 found in index %td!", (res - &arr[0]));
    }

    printf("\n");

    int *res2 = linear_search(&arr[0], ARRAY_SIZE, 16);
    if (res2 == NULL)
    {
        printf("Target 16, not found in array...");
    }
    else
    {
        printf("Target 16 found in index %td!", (res2 - &arr[0]));
    }

    exit(EXIT_SUCCESS);
}
