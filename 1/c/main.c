#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    
    swap(&arr[i + 1], &arr[high]);  
    return i + 1;
}

void quicksort(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int count_occurrences(int *arr, int length, int value) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (arr[i] == value) {
            count++;
        }
    }

    return count;
}

int main() {
    FILE *fptr;

    fptr = fopen("input.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file");
        return 1;
    }

    char ch;
    int line_count = 0;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            line_count++;
        }
    }

    int *row_a = malloc(line_count * sizeof(int));
    int *row_b = malloc(line_count * sizeof(int));

    if (row_a == NULL || row_b == NULL) {
        printf("Memory allocation failed");
        fclose(fptr);
        return 1;
    }

    rewind(fptr);

    char buffer[256];
    int row_a_index = 0;
    int row_b_index = 0;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL) {
        char * pch;
        int column_index = 0;
        pch = strtok (buffer," ");
        while (pch != NULL)
        {
            if (column_index == 0) {
                row_a[row_a_index] = atoi(pch);
                row_a_index++;
            } else if (column_index == 1) {
                row_b[row_b_index] = atoi(pch);
                row_b_index++;
            } else {
                printf("Error: More than 2 columns in the input file\n");
                free(row_a);
                free(row_b);
                fclose(fptr);
                return 1;
            }

            column_index++;
            pch = strtok(NULL, " ");
        }
    }

    fclose(fptr);

    if (row_a_index != row_b_index) {
        printf("Error: Unequal number of elements in the two rows\n");
        free(row_a);
        free(row_b);
        return 1;
    }
    
    quicksort(row_a, 0, row_a_index - 1);
    quicksort(row_b, 0, row_b_index - 1);


    int diff = 0;
    int similarity = 0;
    for (int i = 0; i < row_a_index; i++) {
        similarity += row_a[i] * count_occurrences(row_b, row_b_index, row_a[i]);

        int temp_diff = row_a[i] - row_b[i];
        if (temp_diff < 0) {
            temp_diff = -temp_diff;
        }

        diff += temp_diff;
    }


    free(row_a);
    free(row_b);

    printf("Similarity: %d\n", similarity);
    printf("Difference: %d\n", diff);

    return 0;
}