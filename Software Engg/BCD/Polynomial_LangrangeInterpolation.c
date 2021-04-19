/**
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: SOFTWARE ENGINEERING LAB. (UG2, Sem2)
 * ASSIGNMENT: SECRET INFORMATION RETRIEVAL USING LANGRANGE INTERPOLATION METHOD (QUESTION 3)
 */

/************************************************************************
* COMPILATION: gcc 3.c -o 3
* EXECUTION: ./3.exe
----------------------------------------------------------------------
* Sample I/O: 

Enter number of secret coefficients: 
5

Enter Secret Coefficients: 
1 -2 4 3 -2

Number of x,y pairs to generate: 
10

Pairs generated from the Polynomial: 
X : 1  Y : 4
X : 2  Y : 5
X : 3  Y : -50
X : 4  Y : -263
X : 5  Y : -784
X : 6  Y : -1811
X : 7  Y : -3590
X : 8  Y : -6415
X : 9  Y : -10628
X : 10  Y : -16619

Say, These 10 pairs are distributed to 5 persons
----------------------------------------------------------------
Enter 5 pairs of (X,y) : 
Enter X y: 1 4
Enter X y: 2 5
Enter X y: 8 -6415
Enter X y: 9 -10628
Enter X y: 7 -3590

The Polynomial Coefficients are : 
1 -2 4 3 -2 
-------------------------------------------------------------------------
* Since input keys and output keys are same, we conclude that the program
* has run successfully.
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* utility function to evaluate x raised to the power y */
double power(double x, double y)
{
    double z = 1;
    int i, j;
    for (i = 0; i < y; i++)
    {
        z *= x;
    }
    return z;
}

/** function calculates the values of S[0],S[1]...S[k-1] i.e. secret keys
 * P(x) = (x-x1)(x-x2)(x-x3)...(x-xk) = S[0]*x^k + S[1]*x^k-1 + ..... + S[k-2]*x + S[k-1]  
 */
double *computeTempCoeffs(double **XY, int k)
{
    double *S = (double *)malloc(k * sizeof(double));
    S[0] = 1;
    S[1] = 0;

    int i, j, l;
    for (i = 0; i < k; i++)
    {
        S[1] += XY[i][0];
    }

    for (i = 2; i < k; i++)
    {
        S[i] = 0;
        for (j = 0; j < k; j++)
        {
            double s = XY[j][0];
            for (l = 1; l < i; l++)
            {
                s = S[l] - s;
                s = s * XY[j][0];
            }
            S[i] += s;
        }
        S[i] = S[i] / i;
    }
    return S;
}

/* Calculates the Coefficients of The Lagrange Interpolation for individual xi,yi */
double **computeLangrangeCoeffs(double **XY, double *S, int k)
{
    double **L = (double **)malloc(k * sizeof(double *));
    int i, j, l;
    for (i = 0; i < k; i++)
    {
        L[i] = (double *)malloc(k * sizeof(double));
    }
    for (i = 0; i < k; i++)
    {
        for (j = 0; j < k; j++)
        {
            if (j == 0)
            {
                L[i][j] = S[j];
            }
            else
            {

                double x = XY[i][0];
                for (l = 1; l < j; l++)
                {
                    x = S[l] - x;
                    x = x * XY[i][0];
                }
                L[i][j] = (S[j] - x);
            }
        }
    }
    for (i = 0; i < k; i++)
    {
        double denom = 1.0;
        for (j = 0; j < k; j++)
        {
            if (i != j)
            {
                denom = denom * (XY[i][0] - XY[j][0]);
            }
        }
        for (j = 0; j < k; j++)
        {

            L[i][j] = L[i][j] / denom;
        }
    }
    return L;
}

/* Computes the Coefficients of the polynomial */
double *computePolyCoeffs(double **XY, double **L, int k)
{
    double *P = (double *)malloc(k * sizeof(double));
    int i, j;
    for (i = 0; i < k; i++)
    {
        P[i] = 0;
        for (j = 0; j < k; j++)
        {
            P[i] += (XY[j][1] * L[j][i]);
        }
        if (i % 2 != 0)
        {
            P[i] = P[i] * (-1.0);
        }
    }
    return P;
}

/* printing the coefficients in order of their input */
void printCoeff(double *P, int k)
{
    int i, j;
    printf("\nThe Polynomial Coefficients are : \n");
    for (i = k - 1; i >= 0; i--)
    {
        printf("%0.0lf ", P[i]);
    }
    printf("\n");
}

int main()
{
    // variables to take secret keys input
    int n, k, k_secret, n_generate;
    double *secret;

    printf("Enter number of secret coefficients:\n");
    scanf("%d", &k_secret);

    secret = (double *)malloc(k_secret * sizeof(double));
    int i, j;

    // take secret keys as input
    printf("\nEnter Secret Coefficients:\n");
    for (i = 0; i < k_secret; i++)
    {
        scanf("%lf", &secret[i]);
    }

    // generating X,y pairs from the entered polynomial coefficients
    printf("\nNumber of x,y pairs to generate:\n");
    scanf("%d", &n_generate);
    printf("\nPairs generated from the Polynomial:\n");
    for (i = 1; i <= n_generate; i++)
    {
        double x = i;
        double y = 0;
        for (j = 0; j < k_secret; j++)
        {
            y += (power(x, j) * secret[j]);
        }
        printf("X : %0.0lf  Y : %0.0lf\n", x, y);
    }

    printf("\nSay, These %d pairs are distributed to %d persons\n", n_generate, k_secret);
    printf("----------------------------------------------------------------\n");

    n = k_secret;
    k = k_secret;

    double **XY = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < n; i++)
    {
        XY[i] = (double *)malloc(2 * sizeof(double));
    }

    // take input from user the pairs that have been distributed
    printf("Enter %d pairs of (X,y): \n", n);
    for (i = 0; i < n; i++)
    {
        printf("Enter X y: ");
        double x, y;
        scanf("%lf %lf", &x, &y);
        XY[i][0] = x;
        XY[i][1] = y;
    }

    /* compute temporary secret key coefficients */
    double *S = computeTempCoeffs(XY, k);

    /* compute Langrage Interpolation coefficients */
    double **L = computeLangrangeCoeffs(XY, S, k);

    /* compute polynomial coefficients from the Langrange Interpolation coefficients */
    double *P = computePolyCoeffs(XY, L, k);

    /* print the secret information obtained in order same as of input */
    printCoeff(P, k);

    return 0;
}
