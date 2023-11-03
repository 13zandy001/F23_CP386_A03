#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9
#define NUM_THREADS 11

typedef struct
{
    int row;
    int column;  
} parameters;

int sudoku[SIZE][SIZE] = {0};
int valid[NUM_THREADS] = {0};

void *check_rows(void *param);
void *check_columns(void *param);
void *check_subgrid(void *param);

int main() {

    pthread_t threads[NUM_THREADS];
    parameters *data = NULL;
    int i = 0, j = 0;

    // Read Sudoku puzzle from a file
    FILE *file = fopen("sample_in_sudoku.txt", "r");
    if(file == NULL) {
        printf("Unable to open file!\n");
        return 1;
    }

    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            fscanf(file, "%d", &sudoku[i][j]);
        }
    }
    fclose(file);

    // Create a thread to check rows
    data = (parameters *) malloc(sizeof(parameters));
    data->row = 0;
    data->column = 0;
    pthread_create(&threads[0], 0, check_rows, data);

    // Create a thread to check columns
    data = (parameters *) malloc(sizeof(parameters));
    data->row = 0;
    data->column = 0;
    pthread_create(&threads[1], 0, check_columns, data);

    // Create threads to check 3x3 subgrids
    for(i = 0; i < SIZE; i += 3) {
        for(j = 0; j < SIZE; j += 3) {
            data = (parameters *) malloc(sizeof(parameters));
            data->row = i;
            data->column = j;
            pthread_create(&threads[2 + (i / 3) * 3 + j / 3], 0, check_subgrid, data);
        }
    }

    // Wait for all threads to finish
    for(i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check if Sudoku puzzle is valid
    for(i = 0; i < NUM_THREADS; i++) {
        if(valid[i] == 0) {
            printf("Sudoku puzzle is not valid\n");
            return 1;
        }
    }

    printf("Sudoku puzzle is valid\n");
    return 0;
}

void *check_rows(void *param) {
    parameters *params = (parameters *) param;
    int row = params->row;
    int col = params->column;
    int validation[SIZE] = {0};
    int i = 0, j = 0;

    for(i = row; i < SIZE; i++) {
        for(j = col; j < SIZE; j++) {
            validation[sudoku[i][j] - 1]++;
        }

        for(j = 0; j < SIZE; j++) {
            if(validation[j] != 1) {
                valid[0] = 0;
                pthread_exit(0);
            }
            validation[j] = 0;
        }
    }

    valid[0] = 1;
    pthread_exit(0);
}

void *check_columns(void *param) {
    parameters *params = (parameters *) param;
    int row = params->row;
    int col = params->column;
    int validation[SIZE] = {0};
    int i = 0, j = 0;

    for(i = col; i < SIZE; i++) {
        for(j = row; j < SIZE; j++) {
            validation[sudoku[j][i] - 1]++;
        }

        for(j = 0; j < SIZE; j++) {
            if(validation[j] != 1) {
                valid[1] = 0;
                pthread_exit(0);
            }
            validation[j] = 0;
        }
    }

    valid[1] = 1;
    pthread_exit(0);
}

void *check_subgrid(void *param) {
    parameters *params = (parameters *) param;
    int row = params->row;
    int col = params->column;
    int validation[SIZE] = {0};
    int i = 0, j = 0;

    for(i = row; i < row + 3; i++) {
        for(j = col; j < col + 3; j++) {
            validation[sudoku[i][j] - 1]++;
        }
    }

    for(i = 0; i < SIZE; i++) {
        if(validation[i] != 1) {
            valid[2 + (row / 3) * 3 + col / 3] = 0;
            pthread_exit(0);
        }
    }

    valid[2 + (row / 3) * 3 + col / 3] = 1;
    pthread_exit(0);
}
