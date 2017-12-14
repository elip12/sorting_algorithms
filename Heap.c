/*
    Eli Pandolfo
    Heap.c
    
    Implementation of an array-based min heap.
*/

#include "Heap.h"
#include <stdlib.h>
#include <stdio.h>

// declares the structure for a heap. The final type is exported in Heap.h
struct s_heap
{
    int* A;
    int length;
    int heapsize;
};

int main()
{
    int test[10] = {2,4,6,8,10,1,3,5,7,9};
    // int* test = malloc(sizeof(int) * 10);
    // for(int i = 0; i < 10; i++)
    // {
    //      test[i] = i;
    // }
    //buildHeap(test, 10);
    heapSort(test, 10);
    for(int i = 0; i < 10; i++)
    {
         printf("%d\n", test[i]);
    }
    return 0;
}

// declares memory for a new heap, and initializes its fields. It is important to note that
// buildHeap takes as argument an already existing array, that it then turns into a heap by
// swapping some values. The array must not be dynamically allocated.
// The function runs in O(n).
// length holds the length of the array. heapzise holds the number of elements in the heap, 
// aka the number of nodes in the binary tree. Since the array starts at 0, heapsize = 
// the index of the last element in the array + 1.
Heap* buildHeap(int* array, int length)
{
    Heap* heap = malloc(sizeof(struct s_heap));

    heap->A = array;
    heap->length = length;
    heap->heapsize = length;

    for(int n = (length - 1) / 2; n >= 0; n--)
        heapify(heap, n);
    return heap;
}

// Frees dynamic memory associated with a heap. Assumes the array that the heap is based on
// was not dynamically allocated.
void freeHeap(Heap** pH)
{
    free(*pH);
    *pH = NULL;
}

// recursively make the binary subtree whose root is n into a valid min heap: for any node,
// all values below that node are larger than that node
// precondition: the subtrees rooted at the left and right children of n are valid heaps.
// Since buildHeap builds from the bottom up and heap created with buildheap satisfies
// this precondition.
void heapify(Heap* heap, int n)
{
    int l = left(heap, n);
    int r = right(heap, n);

    int smallest;
    if(l != NIL && l <= heap->heapsize && (heap->A)[l] < (heap->A)[n])
        smallest = l;
    else
        smallest = n;
    if(r != NIL && r <= heap->heapsize && (heap->A)[r] < (heap->A)[smallest])
        smallest = r;
    if(smallest != n)
    {
        int temp = (heap->A)[smallest];
        (heap->A)[smallest] = (heap->A)[n];
        (heap->A)[n] = temp;
        heapify(heap, smallest);
    }
}

// sorts an array of ints
// O(nlogn)
void heapSort(int* A, int length)
{
    Heap* heap = buildHeap(A, length);
    for(int n = (length - 1); n > 0; n--)
    {
        int temp = (heap->A)[n];
        (heap->A)[n] = (heap->A)[0];
        (heap->A)[0] = temp;
        (heap->heapsize)--;
        heapify(heap, 0);
    }
    //reverse array, since it is now sorted in reverse order 
    for(int n = 0; n < length / 2; n++)
    {
        int temp = (heap->A)[n];
        (heap->A)[n] = (heap->A)[(length - 1) - n];
        (heap->A)[(length - 1) - n] = temp;
    }
    freeHeap(&heap);
}

// NIL = -1

//returns the left child of n, or -1 if n has no left child
int left(Heap* heap, int n)
{
    int left = (n + 1) * 2 - 1;
    if(left < heap->heapsize)
        return left;
    else
        return NIL;
}

int right(Heap* heap, int n)
{
    int right = (n + 1) * 2;
    if(right < heap->heapsize)
        return right;
    else
        return NIL;
}

int parent(int n)
{
    if(n == 0)
        return NIL;
    else return n / 2;
}
