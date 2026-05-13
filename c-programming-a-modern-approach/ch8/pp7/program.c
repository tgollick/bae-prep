#include <stdio.h>

int main()
{
    int arr[5][5] = {0};

    for (int i = 0; i < 5; i++)
    {
        printf("Enter row %d: ", i + 1);
        for (int j = 0; j < 5; j++)
        {
            scanf("%d", &arr[i][j]);
        }
    }

    printf("\n");

    printf("Row totals: ");
    for (int i = 0; i < 5; i++)
    {
        int row_total = 0;

        for (int j = 0; j < 5; j++)
        {
            row_total += arr[i][j];
        }

        printf("%d ", row_total);
    }

    printf("\n");

    printf("Column totals: ");
    for (int i = 0; i < 5; i++)
    {
        int column_total = 0;

        for (int j = 0; j < 5; j++)
        {
            column_total += arr[j][i];
        }

        printf("%d ", column_total);
    }

    return 0;
}