/*
    p_mergesort.c
    Eli Pandolfo

    A parallel implementation of max, using the pthreads library.
    Finds the max element in a large array using multithreading.
    At a certain point, the overhead associated with using more threads, as well as
    the limited number of cores of the computer this program runs on, cancels out the benefit
    of parallelism.

    For my 4-core machine (an an input array of size 1M numbers), this is around 8 threads.
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_THREADS 8
#define ARRAY_SIZE 1000000
#define EXTRASTACK 8388608 // 4 MB

// function that linearly finds the max element in an array
void *max(void *A)
{
    int i;
    int t;
    int result;
    int *ret;
    int size;

    t = ((int *)A)[0];
    result = ((int *)A)[1];
    
    size = ARRAY_SIZE / NUM_THREADS;
    if (t == NUM_THREADS - 1)
    {
        size += ARRAY_SIZE % NUM_THREADS;
    }
    //printf("SIZE: %d\n", size);
    for(i = 1; i < size; i++)
    {
        if(((int *)A)[i + 1] > result)
        {
            result = ((int *)A)[i + 1];
        }
    }
    //printf("Thread: result = %d\n", result);

    //free allocated memory input
    free(A);

    ret = (int *)malloc(sizeof(int));
    ret[0] = result;
    return (void *) ret;
}

int main()
{
    size_t stacksize;
    pthread_t thread[NUM_THREADS];
    int A[ARRAY_SIZE];
    int *thread_A;
    pthread_attr_t attr;
    int rc;
    void *thread_ret;
    long t;
    int a;
    int result;
    time_t seed;


    // initialize array
    /* Intializes random number generator */
    srand((unsigned) time(&seed));
    
    for(a = 0; a < ARRAY_SIZE; a++)
    {
        A[a] = rand();
    }
    result = A[0];

    // initialize threads and set joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    stacksize = sizeof(int) * (ARRAY_SIZE / NUM_THREADS) + EXTRASTACK;
    pthread_attr_setstacksize(&attr, stacksize);

    for(t = 0; t < NUM_THREADS; t++)
    {
        int size;

        //printf("Main: creating thread %d\n", t);
        size = ARRAY_SIZE / NUM_THREADS;
        if (t == NUM_THREADS - 1)
        {
            size += ARRAY_SIZE % NUM_THREADS;
        }
        thread_A = (int *)malloc(sizeof(int) * (size + 1));
        thread_A[0] = t;
        for(int i = 0; i < size; i++)
        {
            thread_A[i + 1] = A[t * (ARRAY_SIZE / NUM_THREADS) + i];
        }
        rc = pthread_create(&thread[t], &attr, max, (void *)thread_A);
        if (rc != 0)
        {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // free attribute, wait for threads to complete, then join them to main
    pthread_attr_destroy(&attr);
    for(t = 0; t < NUM_THREADS; t++)
    {
        rc = pthread_join(thread[t], &thread_ret);
        if (rc != 0)
        {
            printf("ERROR: return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        //printf("Main: thread %d exit status: %d\n", t, rc);
        //printf("Main: thread %d return value: %d\n", t, ((int *)thread_ret)[0]);
        if (((int *)thread_ret)[0] > result)
        {
            result = ((int *)thread_ret)[0];
        }
        free(thread_ret);
    }

    printf("Main: result = %d\n", result);

    //printf("Main: program completed. Exiting\n");
    pthread_exit(0);
}