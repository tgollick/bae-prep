#include <stdio.h>

void split_time(long total_sec, int *hr, int *min, int *sec);

int main(void)
{
    int hr, min, sec;
    long total_sec;

    printf("Enter a number of seconds: ");
    scanf("%ld", &total_sec);

    split_time(total_sec, &hr, &min, &sec);

    printf("\nHours: %d\n", hr);
    printf("Minutes: %d\n", min);
    printf("Seconds: %d\n", sec);

    return 0;
}

void split_time(long total_sec, int *hr, int *min, int *sec)
{
    *hr = total_sec / 3600;
    total_sec -= (*hr * 3600);
    *min = total_sec / 60;
    total_sec -= (*min * 60);
    *sec = total_sec;
}
