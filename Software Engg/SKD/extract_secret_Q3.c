/**
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: SOFTWARE ENGG. LAB (UG2, Sem2)
 * ASSIGNMENT: INFORMATION EMBEDDING AND RETRIEVAL (ASSIGNMENT 2), QUESTION 3
 */

#include <stdio.h>
#include <stdlib.h>

int getKthLSB(int, int);
void printInformation(char *);

int main(int argc, char **argv)
{
    int rows, cols;
    if (argc < 3)
    {
        /* command line arguments not provided, prompt the user to input */
        printf("Enter dimensions of matrix having embedded information:\n");
        scanf("%d %d", &rows, &cols);
    }
    else
    {
        /* command line arguments provided by user */
        rows = atoi(argv[1]);
        cols = atoi(argv[2]);
    }
    int i, j;

    int **mat = (int **)malloc(rows * sizeof(int **));

    for (i = 0; i < rows; i++)
    {
        mat[i] = (int *)malloc(cols * sizeof(int *));
    }

    /* taking matrix as input from user */
    printf("Enter the matrix with embedded information:\n");
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            scanf("%d", &mat[i][j]);
        }
    }

    /* take LSB position as input */
    int bit;
    printf("Enter how many bit LSB is to be taken: \n");
    scanf("%d", &bit);

    char *information = (char *)malloc((rows * cols * bit) * sizeof(char));
    int idx = 0;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            int embed = mat[i][j];
            int k;
            for (k = bit; k > 0; k--)
            {
                int info = getKthLSB(embed, k);
                information[idx++] = (char)(info + 48);
            }
        }
    }
    information[idx] = '\0';
    printInformation(information);
    return 0;
}

int getKthLSB(int x, int k)
{
    while (k > 1)
    {
        x >>= 1;
        k--;
    }
    return (x & 1);
}

void printInformation(char *info)
{
    printf("Information obtained from the given matrix: \n");
    printf("%s\n", info);
}