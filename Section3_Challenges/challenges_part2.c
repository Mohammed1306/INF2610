/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Challenges - part2.c
 *
 * Mohammed Abdellatif Kallel 2292761, Joey Hasrouny 2185301
*/
#include "challenges_part2.h"

typedef struct {
    Matrix* A;
    Matrix* B;
    Matrix* C;
    int row;
} ThreadParameters;


void initialiseMatrix(Matrix* initialMatrix, int rows, int cols) {
    initialMatrix -> rows = rows;
    initialMatrix -> cols = cols;
    initialMatrix -> matrix = (short**)malloc(rows * sizeof(short*));
    for (int i = 0; i < rows; i++) {
        initialMatrix -> matrix[i] = (short*)malloc(cols * sizeof(short));
    }
}

/*
void freeMatrix(Matrix* matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->matrix[i]);
    }
    free(matrix->matrix);
}
*/

void* calculateRows(void* arg) {
    ThreadParameters* data = (ThreadParameters*) arg;
    int row = data -> row;

    for (int k = 0; k < data -> B -> cols; k++) {
        data -> C -> matrix[row][k] = 0;
        for (int j = 0; j < data -> A -> cols; j++) {
            data -> C -> matrix[row][k] += data -> A -> matrix[row][j] * data -> B -> matrix[j][k];
        }
    }

    pthread_exit(0);
}

Matrix* multiply(Matrix* A, Matrix* B) {

    if (!A || !B || A -> cols != B -> rows)  {
        return NULL;
    }

    Matrix* C = (Matrix*)malloc(sizeof(Matrix));
    initialiseMatrix(C, A -> rows, B -> cols);

    pthread_t threads[A -> rows];
    ThreadParameters thread_parameters[A -> rows];

    for (int i = 0; i < A -> rows; i++) {
        thread_parameters[i].A = A;
        thread_parameters[i].B = B;
        thread_parameters[i].C = C;
        thread_parameters[i].row = i;
        pthread_create(&threads[i], NULL, calculateRows, (void*)&thread_parameters[i]);
    }

    for (int i = 0; i < A -> rows; i++) {
        pthread_join(threads[i], NULL);
    }

    return C;
}

/*
void printMatrix(Matrix* matrix) {
    for (int i = 0; i < matrix -> rows; i++) {
        for (int k = 0; k < matrix -> cols; k++) {
            printf("%d ", matrix -> matrix[i][k]);
        }
        printf("\n");
    }
}


// This main is to help you in your development.
// Don't forget to comment it out when you run the tests.


int main(int argc, char*argv[])
{
    Matrix A, B;
    short nRows = 2;
    short nCols = 2;
    initialiseMatrix(&A, nRows, nCols);
    A.matrix[0][0] = 1;
    A.matrix[0][1] = 2;
    A.matrix[1][0] = 3;
    A.matrix[1][1] = 4;

    initialiseMatrix(&B, nRows, nCols);
    B.matrix[0][0] = 5;
    B.matrix[0][1] = 6;
    B.matrix[1][0] = 7;
    B.matrix[1][1] = 8;

    Matrix* C = multiply(&A, &B);
    printMatrix(C);

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(C);
    return 0;   
} */

