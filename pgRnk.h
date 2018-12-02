#ifndef PGRNK_H
#define PGRNK_H
#define _CRT_SECURE_NO_WARNINGS
#ifdef _OPENMP
#include </usr/local/Cellar/gcc/8.2.0/lib/gcc/8/gcc/x86_64-apple-darwin18.0.0/8.2.0/include/omp.h>
#endif
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef enum coinface {
	TAILS, HEADS
}COINFACE;

COINFACE tossWeightedCoin(double dampingVal);
void printGraph(Graph *g);
void readGraph(Graph *graphObject, char *filename);
void printUsage();
void findMax5(int *arr, int size);

#endif
