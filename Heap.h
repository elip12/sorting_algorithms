/*
    Eli Pandolfo
    Heap.h
    
    Header file for an array-based min heap.
*/

#ifndef HEAP_H
#define HEAP_H

#define NIL -1

typedef struct s_heap Heap;

int parent(int i);
int left(Heap* h, int i);
int right(Heap* h, int i);

void heapify(Heap* h, int i);
Heap* buildHeap(int* array, int length);
void freeHeap(Heap** pH);
void heapSort(int* h, int length);

#endif