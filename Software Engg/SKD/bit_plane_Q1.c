/**
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: SOFTWARE ENGG. LAB (UG2, Sem2)
 * ASSIGNMENT: INFORMATION EMBEDDING AND RETRIEVAL (ASSIGNMENT 2), QUESTION 1
 */

#include <stdio.h>
#include <stdlib.h>

const int NUM_BITS = 8;

int **initializeMatrix(int, int);
void printPlane(int **, int, int, int);

int main(int argc, char **argv)
{

    int rows, cols;
    printf("Enter dimensions of matrix :\n");
    scanf("%d %d", &rows, &cols);

    int **mat = initializeMatrix(rows, cols);

    printf("Enter the elements of the matrix:\n");
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            scanf("%d", &mat[i][j]);
        }
    }

    int pIndex;
    for (pIndex = 0; pIndex < NUM_BITS; pIndex++)
    {
        int **plane = initializeMatrix(rows, cols);
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                plane[i][j] = (mat[i][j] & (1 << pIndex));
            }
        }
        printPlane(plane, rows, cols, pIndex);
        printf("-----------------------------------------\n");
    }
    return 0;
}

int **initializeMatrix(int rows, int cols)
{
    int i;
    int **mat = (int **)malloc(rows * sizeof(int **));
    for (i = 0; i < rows; i++)
    {
        mat[i] = (int *)malloc(cols * sizeof(int *));
    }
    return mat;
}

void printPlane(int **plane, int rows, int cols, int pIndex)
{
    printf("Bit Plane for bit plane index %d:\n", pIndex);
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            printf("%d ", plane[i][j]);
        }
        printf("\n");
    }
}
