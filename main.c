// Matthew Ferran
//
#ifdef _OPENMP
#include </usr/local/Cellar/gcc/8.2.0/lib/gcc/8/gcc/x86_64-apple-darwin18.0.0/8.2.0/include/omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void printGraph(Graph *g){
	int i = 0;
	for (i = 0; i < g->numNodes; i++){
		printf("( %d : ", i);
		//printList(*((g->graph)+i));
		printf(" )\n");
	}
}


void readGraph(Graph *graphObject, char *filename){
	FILE *infile = NULL;
	char buf[50];
	int u, v;
	List *pCur = NULL;




	printf("Debug: fopen\n");
	infile = fopen(filename, "r");

	//step 1: determine then number of nodes.
	

	//read each line
	if (!infile){ //file failed to open
		printf("ERROR: file open failed.\n");
		exit(1);
	}
	else {
		//if the graph is ordered, read the last line, allocate memory once, and read the file
		
		printf("Debug: fseek\n");
		fseek(infile, -50, SEEK_END);
		printf("Debug: fgets\n");
		fgets(buf, sizeof(buf), infile);
		printf("Debug: fseek2\n");
		fseek(infile, 0, 0);
		printf("Debug: sscanf\n");
		sscanf(buf, "%d %d", &u, &v);
		printf("Debug: calloc(u[%d], sizeof(List)[%d]\n", u, (int)sizeof(List));
		graphObject->nodeArray = (List *) malloc(u * sizeof(List));
		if (!graphObject->nodeArray) {
			printf("calloc returned null\n");
			return;
		}
		printf("Debug: set numnodes\n");
		graphObject->numNodes = u;
	}
	while (!feof(infile)){
		if (!fgets (buf, sizeof(buf), infile)) { break; }//if null just end
		//else
		// LINE: nodeID destID
		sscanf(buf, "%d %d", &u, &v);

		//printf("%d %d\n", u, v);
		if (u > graphObject->numNodes){ //need to resize array
			graphObject->nodeArray = (List *) realloc(graphObject->nodeArray, sizeof(List) * u);
			//zero the memory out there
			memset(graphObject->nodeArray+(graphObject->numNodes), 0, sizeof(List)*(u-graphObject->numNodes));
			graphObject->numNodes = u;
		}
		pCur = graphObject->nodeArray + (u-1);
		insertFront(pCur, v);// insert this edge into the array
	}
	fclose(infile);
	
}

void printUsage(){
	printf("Usage - ");
	printf("pagerank {K: length of random walk} {D: damping ratio} {F: graph file}\n");
}

int main(int argc, char *argv[]){
	int K = 0;
	double D =  0.0;
	char *F;
	Graph g;

#ifdef _OPENMP
	int rank = omp_get_thread_num();
	int thread_count = omp_get_num_threads();
#else
	int rank = 0;
	int thread_count = 1;
#endif
	if (argc != 4){ //incorrect number of input arguments
		printUsage();
		exit(1);
	}

	printf("Rank: %d, #threads: %d\n", rank, thread_count);
	K = atoi(argv[1]);
	D = atof(argv[2]);
	F = strdup(argv[3]);

	readGraph(&g, F);
	printGraph(&g);
	// for (every node in the graph) walk
		//curr <- node i
		//for (step = 1 to L){
			//visit[curr]++;
			//toss a coin
			//if (heads){
				//successor = pick a random node between 1 and n (each with prob 1/n)
				//curr <- succ;
			//else {// w/prob 1-D
				//succ <- pick a random node from one of curr's out links
			//}
		//}

}
