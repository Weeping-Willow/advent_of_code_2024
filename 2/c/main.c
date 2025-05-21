#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool valid_row(int row[], int row_len)
{
    int previous_status = -1;

    for (int i = 0; i < row_len - 1; i++)
    {

        int status = 0;
        if (row[i] == row[i + 1])
        {
            status = 1;
        }

        if (row[i] > row[i + 1])
        {
            status = 2;
        }

        if (previous_status == -1)
        {
            previous_status = status;
        }

        int diff = row[i] - row[i + 1];
        bool allowed_diff = ((diff >= -3 && diff <= -1) || (diff >= 1 && diff <= 3));
        if (!allowed_diff)
        {
            return false;
        }

        if (status != previous_status)
        {
            return false;
        }
    }

    return true;
}

void remove_element(const int *arr, int len, int s, int *out)
{
    int out_idx = 0;
    for (int i = 0; i < len; i++)
    {
        if (i == s)
            continue;
        out[out_idx++] = arr[i];
    }
}

int main()
{
    FILE *fptr;

    fptr = fopen("input.txt", "r");
    if (fptr == NULL)
    {
        printf("Error opening file");
        return 1;
    }

    char buffer[256];
    int line_count = 0;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        line_count++;
    }

    int **data = malloc(line_count * sizeof(int *));
    int *column_counts = malloc(line_count * sizeof(int));
    if (data == NULL || column_counts == NULL)
    {
        printf("Memory allocation failed\n");
        fclose(fptr);
        free(data);
        free(column_counts);
        return 1;
    }

    rewind(fptr);

    int index = 0;
    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        // Make a copy of buffer since strtok modifies it
        char buffer_copy[256];
        strncpy(buffer_copy, buffer, sizeof(buffer_copy));
        buffer_copy[sizeof(buffer_copy) - 1] = '\0';

        // First pass: count columns
        int column_count = 0;
        char *pch = strtok(buffer_copy, " ");
        while (pch != NULL)
        {
            column_count++;
            pch = strtok(NULL, " ");
        }

        column_counts[index] = column_count;
        data[index] = malloc(column_count * sizeof(int));
        if (data[index] == NULL)
        {
            printf("Memory allocation failed\n");
            for (int i = 0; i < index; i++)
            {
                free(data[i]);
            }
            free(data);
            free(column_counts);
            fclose(fptr);
            return 1;
        }

        // Second pass: parse numbers
        int current_index = 0;
        pch = strtok(buffer, " ");
        while (pch != NULL && current_index < column_count)
        {
            data[index][current_index++] = atoi(pch);
            pch = strtok(NULL, " ");
        }

        index++;
    }

    fclose(fptr);

    printf("got it: %d\n", index);
    int count_loose = 0;
    int count_strict = 0;
    for (int i = 0; i < index; i++)
    {
        int column_count = column_counts[i];

        if (valid_row(data[i], column_count))
        {
            count_loose++;
            count_strict++;
            continue;
        }

        for (int j = 0; j < column_count; j++)
        {
            int *new_row = malloc((column_count - 1) * sizeof(int));
            if (!new_row)
                continue;
            remove_element(data[i], column_count, j, new_row);
            if (valid_row(new_row, column_count - 1))
            {
                count_loose++;
                free(new_row);
                break;
            }
            free(new_row);
        }
    }

    for (int i = 0; i < index; i++)
    {
        free(data[i]);
    }

    free(data);

    printf("Loose: %d\n", count_loose);
    printf("Strict: %d\n", count_strict);

    return 0;
}