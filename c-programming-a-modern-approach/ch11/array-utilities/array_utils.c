#include "array_utils.h"
#include <stdio.h>

int *max_element(int *arr, int n)
{
    int *max = arr;

    for (int *p = arr; p < arr + n; p++)
    {
        if (*max < *p)
        {
            max = p;
        }
    }

    return max;
}

int *min_element(int *arr, int n)
{
    int *min = arr;

    for (int *p = arr; p < arr + n; p++)
    {
        if (*min > *p)
        {
            min = p;
        }
    }

    return min;
}

void swap(int *a, int *b)
{
    const int temp = *a;
    *a = *b;
    *b = temp;
}

void reverse_in_place(int *arr, int n)
{
    int *front = arr;
    int *back = arr + n - 1;

    while (front < back)
    {
        swap(front, back);
        front++;
        back--;
    }
}

int *linear_search(int *arr, int n, int target)
{
    for (int *p = arr; p < arr + n; p++)
    {
        if (*p == target)
        {
            return p;
        }
    }

    return NULL;
}

void print_arr(int *arr, int n)
{
    for (int *p = arr; p < arr + n; p++)
    {
        printf("%d ", *p);
    }

    printf("\n");
}