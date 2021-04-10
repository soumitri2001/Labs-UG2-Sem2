/**
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: SOFTWARE ENGG. LAB (UG2, Sem2)
 * ASSIGNMENT: INFORMATION EMBEDDING AND RETRIEVAL (ASSIGNMENT 2), QUESTION 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_BYTES 1000000

int embedInfo(int, int, int);
int getInfoSegment(char *, int, int);

int main(int argc, char **argv)
{
    char *info = (char *)malloc(MAX_BYTES * sizeof(char));
    printf("Enter the information bitstring:\n");
    scanf("%s", info);

    int bit;
    printf("Enter how many bit LSB is to be taken:\n");
    scanf("%d", &bit);

    int bitlen = strlen(info);

    if (bitlen % bit != 0)
    {
        printf("Bitstring length must be divisible by number of bit LSB to be taken");
        exit(1);
    }

    int rows, cols;
    printf("Enter dimensions of matrix for embedding information:\n");
    scanf("%d %d", &rows, &cols);

    if (rows * cols != (bitlen / bit))
    {
        printf("Matrix dimensions should equal the effective number of bits in bitstring");
        exit(1);
    }

    int i, j;
    int **mat = (int **)malloc(rows * sizeof(int **));
    for (i = 0; i < rows; i++)
    {
        mat[i] = (int *)malloc(cols * sizeof(int *));
    }

    printf("Enter the elements of the matrix:\n");
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            scanf("%d", &mat[i][j]);
        }
    }

    int idx = 0;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            int k;
            int sub_info = getInfoSegment(info, idx, bit);
            mat[i][j] = embedInfo(mat[i][j], sub_info, bit);
            idx += bit;
        }
    }

    printf("Matrix after information is embedded:\n");
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }

    return 0;
}

int embedInfo(int base, int info, int bit)
{
    int n_bit = 1 + (int)log2(base);
    base = base & (((1 << (n_bit - bit)) - 1) << bit);
    return (base + info);
}

int getInfoSegment(char *info, int start, int bits)
{
    int k;
    int sub_info = 0;
    for (k = bits - 1; k >= 0; k--)
    {
        if (info[start + k] == '1')
            sub_info += (1 << (bits - 1 - k));
    }
    return sub_info;
}
