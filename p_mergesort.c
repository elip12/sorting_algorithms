/*
    p_mergesort.c
    Eli Pandolfo

    Parallel mergesort using pthreads.

    Algorithm runs in O(nlog(n) / x + nx), where n is the size of input 
    and x is the number of threads used.
    
    By calculation and testing: most efficient parameters: x = root(log(n))

    Each thread does standard mergesort on 1/x of the original array. Results are merged
    as they are computed.

    There is some messiness with only using void * for arrays + structures, but I want to be able to
    generalize to arrays of anything at a later date. Also, pthread only lets you input and output
    void pointers to threads, so a lot of casting is inevitable.

    TODO:   generalize to any data type
            speed up final merging with heap
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define ARRAY_SIZE 10000000
#define NUM_THREADS ((int)(round(sqrt(log(ARRAY_SIZE) / log(2)))))
#define EXTRASTACK 1000000 // ~1 MB

/*  structure for an array with a size
    makes life a lot easier */
typedef struct s_array
{
    long size;
    void *data;

} array;

/*  prints an array of intsgiven its size
    in: void pointer to array, size
    out: void */
void print_array(void *A, long size)
{
    long i;

    for(i = 0; i < size; i++)
    {
        printf("%d ", ((int *)A)[i]);
    }
    printf("\n");
}

/*  Takes in 3 arrays: output, input 1, input 2
    All arrays must be allocated before hand with their correct sizes
    merges input 1 and input 2 into output.
    in: output, input1, size1, input2, size2

    called both by the created threads and by the main */
void *merge(void *A, void *B, long sizeB, void *C, long sizeC)
{
    long Aindex;
    long Bindex;
    long Cindex;

    Bindex = 0;
    Cindex = 0;
    for(Aindex = 0; Aindex < sizeB + sizeC; Aindex++)
    {
        if((Bindex != sizeB) && (Cindex == sizeC || ((int *)B)[Bindex] <= ((int *)C)[Cindex]))
            ((int *)A)[Aindex] = ((int *)B)[Bindex++];
        else
            ((int *)A)[Aindex] = ((int *)C)[Cindex++];
    }

    return A;
}

/*  recursive sort algorithm for mergesort
    in: array with a size
    out: same (modified) array */
void *sort(void *A, long size)
{
    void *B;
    long sizeB;
    void *C;
    long sizeC;
    int i;

    // base case
    if (size == 1)
        return A;
    
    // partition array in halves
    sizeB = size / 2;
    sizeC = size - sizeB;

    B = malloc(sizeof(int) * sizeB);
    for(i = 0; i < sizeB; i++)
    {
        ((int *)B)[i] = ((int *)A)[i];
    }

    C = (int *)malloc(sizeof(int) * sizeC);
    for(i = 0; i < sizeC; i++)
    {
        ((int *)C)[i] = ((int *)A)[i + sizeB];
    }

    // recursively sort halves
    B = sort(B, sizeB);
    C = sort(C, sizeC);

    // merge sorted halves
    merge(A, B, sizeB, C, sizeC);
    
    // free allocated memory
    free(B);
    B = NULL;
    free(C);
    C = NULL;

    return A;
}

void *start(void *A)
{
    void *data;
    long size;

    data = ((array *)A)->data;
    size = ((array *)A)->size;

    // since data is a pointer, changes to data are reflected in A
    data = sort(data, size);
    
    return (void *)A;
}

/*
    stacksize: the size of the stack given to each created thread.
    thread: array thread IDs (which are really just addresses in an array)
    A: the input array
    attr: pthread attribute that defines thread properties
    rc: return value for thread creation and joining
    thread_ret: the pointer to an array struct returned by each thread
    t: index for threads
    a: index for misc for loops
    seed: seed for the random number generator
    temp: temp array for merging sorted arrays returned by threads
    output: final output array

    main program creates an array, fills it with random numbers, partitions it,
    generates threads that sort each partition, then merges the sorted partitions when the threads
    finish with them.

    threads can operate in parallel, allowing for faster sorting.
*/
int main()
{
    size_t stacksize;
    pthread_t thread[NUM_THREADS];
    array *A;
    array *thread_A;
    pthread_attr_t attr;
    int rc;
    void *thread_ret;
    int t;
    int a;
    time_t seed;
    array *temp;
    array *output;

    printf("NUM_THREADS: %d\n", NUM_THREADS);

    // initialize random number generator and array
    srand((unsigned) time(&seed));
    A = (array *)malloc(sizeof(array));
    A->size = ARRAY_SIZE;
    A->data = malloc(sizeof(int) * ARRAY_SIZE);
    for(a = 0; a < ARRAY_SIZE; a++)
    {
        //A[a] = ARRAY_SIZE - a;
        ((int *)A->data)[a] = rand();
    }
    
    // initialize threads and set joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    stacksize = sizeof(int) * (ARRAY_SIZE / NUM_THREADS) + EXTRASTACK;
    pthread_attr_setstacksize(&attr, stacksize);

    for(t = 0; t < NUM_THREADS; t++)
    {
        long size;

        size = ARRAY_SIZE / NUM_THREADS;
        if (t == NUM_THREADS - 1)
        {
            size += ARRAY_SIZE % NUM_THREADS;
        }
        
        // instantiate array struct to pass to thread
        thread_A = malloc(sizeof(array));
        thread_A->size = size;
        thread_A->data = malloc(sizeof(int) * size);
        
        // copy desired partition of input array into thread array
        for(a = 0; a < size; a++)
        {
            ((int *)(thread_A->data))[a] = ((int *)A->data)[t * (ARRAY_SIZE / NUM_THREADS) + a];
        }
        
        // create thread
        printf("Main: creating thread %d\n", t);
        rc = pthread_create(&thread[t], &attr, start, (void *)thread_A);
        if (rc != 0)
        {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }

        // thread_A does not need to be freed here because it is the pointer that is returned
        // by the threads, so it gets freed after merging.
    }

    // free input array
    free(A->data);
    free(A);
    A = NULL;

    // free attribute, wait for threads to complete, then join them to main and create output
    pthread_attr_destroy(&attr);
    for(t = 0; t < NUM_THREADS; t++)
    {
        // join thread to main when it finishes sorting
        rc = pthread_join(thread[t], &thread_ret);
        if (rc != 0)
        {
            printf("ERROR: return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("Main: thread %d exit status: %d\n", t, rc);
        
        // merge sorted arrays as they come
        if (t == 0)
        {
            // allocate memory for temp and output, copy thread_ret into temp
            output = malloc(sizeof(array));
            output->data = NULL;
            temp = malloc(sizeof(array));
            temp->size = ((array *)thread_ret)->size;
            temp->data = malloc(sizeof(int) * temp->size);
            for (a = 0; a < temp->size; a++)
            {
                ((int *)temp->data)[a] = ((int *)((array *)thread_ret)->data)[a];
            }
        }
        else
        {
            if (output->data != NULL)
            {
                free(output->data);
                output->data = NULL;
            }
            
            // create output array
            output->size = ((array *)thread_ret)->size + temp->size;
            output->data = malloc(sizeof(int) * output->size);
            
            // merge temp and current thread return and store the merged array in output
            // merge frees temp->data and thread_ret->data
            merge(output->data, temp->data, temp->size, ((array *)thread_ret)->data, ((array *)thread_ret)->size);

            // free allocated memory
            free(((array *)thread_ret)->data);
            free(thread_ret);
            thread_ret = NULL;
            free(temp->data);

            // copy output into temp for next iteration
            if (t != NUM_THREADS - 1)
            {
                temp->data = malloc(sizeof(int) * output->size);
                temp->size = output->size;
                for (a = 0; a < output->size; a++)
                {
                    ((int *)temp->data)[a] = ((int *)output->data)[a];
                }
            }
        }
    }

    //printf("Output:\n");
    //print_array(output->data, output->size);

    // free remaining allocated memory
    free(temp);
    free(output->data);
    free(output);

    printf("Main: program completed. Exiting\n");
    pthread_exit(0);
}