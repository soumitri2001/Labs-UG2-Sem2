/**
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: SOFTWARE ENGG. LAB
 * ASSIGNMENT: MATRIX MULTIPLICATION USING A PARALLEL ALGORITHM
 */

/*-----------------------------------------------------------------------------------------------*/

/* 
   INPUT    :   4 4 1
  
   OUTPUT   :   Matrix 1:
                1 1 0 0
                0 1 0 0
                1 1 1 0
                1 1 0 1
                Matrix 2:
                0 1 1 0
                0 0 1 1
                1 0 1 0
                1 0 1 1
                Matrix Product:
                0 1 2 1
                0 0 1 1
                1 1 3 1
                1 1 3 2
                Time taken for the matrix multiplication using 4 threads = 0.001631 seconds
    
    
    COMPILE COMMAND   :  gcc matrix_mult.c -o matrix_mult -lpthread
    EXECUTION COMMAND :  ./matrix_mult 4 4 1

*/

/*---------------------------------------------------------------------------------------------------*/

// header files
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int thread; // stores current working thread that is incremented for every step

/* structure to hold the two matrices, their dimension, their product and nos of cpus */
struct matrices
{
    int **mat1;   // first matrix
    int **mat2;   // second matrix
    int **prod;   // product matrix
    int dim;      // dimension of square matrix
    int num_cpus; // nos of cpus i.e. threads to be used by user
};

typedef struct matrices matrices;

/* utility function to initialize matrices mat1,mat2,prod */
void initializeMatrices(matrices *mat_struct)
{
    int dim = mat_struct->dim;

    // dynamic memory allocation for the matrices
    mat_struct->mat1 = (int **)malloc(dim * sizeof(int *));
    mat_struct->mat2 = (int **)malloc(dim * sizeof(int *));
    mat_struct->prod = (int **)malloc(dim * sizeof(int *));

    for (int i = 0; i < dim; i++)
    {
        // dynamic memory allocation for each row of matrices
        (mat_struct->mat1)[i] = (int *)malloc(dim * sizeof(int));
        (mat_struct->mat2)[i] = (int *)malloc(dim * sizeof(int));
        (mat_struct->prod)[i] = (int *)malloc(dim * sizeof(int));
    }

    srand(time(0)); // setting seed for random number generation, the numbers will be different for every run of the program

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            // any integer modulo 2 results in either 0 or 1
            (mat_struct->mat1)[i][j] = rand() % 2;
            (mat_struct->mat2)[i][j] = rand() % 2;

            // product matrix to be initialized with 0s
            (mat_struct->prod)[i][j] = 0;
        }
    }
}

/* utility function to perform matrix multiplication on a given thread */
void *matrixMult_thread(void *arg)
{
    matrices *mat_args = (matrices *)arg;
    int dim = mat_args->dim;
    int num_cpus = mat_args->num_cpus;
    int cur_thr = thread;
    int num_rows = dim / num_cpus;

    // perfom matrix multiplication on the rows specific to that thread only
    for (int i = cur_thr * num_rows; i < (cur_thr + 1) * num_rows; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            for (int k = 0; k < dim; k++)
            {
                (mat_args->prod)[i][j] += (mat_args->mat1)[i][k] * (mat_args->mat2)[k][j];
            }
        }
    }

    thread++; // going to next thread

    pthread_exit(0); // exit thread
}

/* utility function to print matrix */
void printMatrix(int **mat, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

/* main() method */
int main(int argc, char **argv)
{

    if (argc != 4)
    {
        // parameter mismatch
        printf("Arguments of %s should be: <nos_cpu> <dimension> <out_flag>", argv[0]);
        exit(0);
    }

    // extracting command line input arguments
    int num_cpus = atoi(argv[1]);
    int dim = atoi(argv[2]);
    int flag = atoi(argv[3]);

    // if the matrix cannot be split equally among the CPUs, print appropriate error message and exit
    if (dim % num_cpus != 0)
    {
        printf("Dimension of square matrices should be divisible by no. of CPUs for even distribution across threads.");
        exit(0);
    }

    // create structure and store relevant values
    matrices *mat_struct = (matrices *)malloc(sizeof(matrices));
    mat_struct->dim = dim;
    mat_struct->num_cpus = num_cpus;
    thread = 0;

    initializeMatrices(mat_struct); // initialising the 2 random matrices and the product matrix

    /* threading related code starts here */

    pthread_t thread_ids[num_cpus]; // creating array of thread IDs

    // initialising variables for evaluating execution time
    struct timeval start, end;
    double time_taken;

    // start timer
    gettimeofday(&start, NULL);

    // loop over all threads for parallel processing
    for (int i = 0; i < num_cpus; i++)
    {
        pthread_attr_t attr; // create attribute for thread
        pthread_attr_init(&attr);
        pthread_create(&thread_ids[i], &attr, matrixMult_thread, (void *)mat_struct); // creating thread
    }

    // waiting for the threads to complete working
    for (int i = 0; i < num_cpus; i++)
    {
        pthread_join(thread_ids[i], NULL);
    }

    // end timer
    gettimeofday(&end, NULL);

    // evaluate execution time
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken += (end.tv_usec - start.tv_usec) * 1e-6;

    // if output flag equals 1, print all matrices
    if (flag == 1)
    {
        printf("Matrix 1:\n");
        printMatrix(mat_struct->mat1, mat_struct->dim);

        printf("Matrix 2:\n");
        printMatrix(mat_struct->mat2, mat_struct->dim);

        printf("Matrix Product:\n");
        printMatrix(mat_struct->prod, mat_struct->dim);
    }

    // print execution time of code
    printf("Time taken for the matrix multiplication using %d threads = %6f seconds\n", num_cpus, time_taken);
}