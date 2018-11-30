// Matthew Ferran
//
#ifdef _OPENMP
#include </usr/local/Cellar/gcc/8.2.0/lib/gcc/8/gcc/x86_64-apple-darwin18.0.0/8.2.0/include/omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib-2.0/glib.h>


typedef struct Node {
	int degree;
	GSList * outgoing;
	int visited;
} node;

void readGraph(char *filename){
	FILE *infile = NULL;
	char buf[50];
	int u, v;

	GSList **graph = NULL;

	infile = fopen(filename, "r");

	//read each line
	if (!infile){ //file failed to open
		printf("ERROR: file open failed.\n");
		exit(1);
	}
	while (!feof(infile)){
		if (!fgets (buf, sizeof(buf), infile)) { break; }//if null just end
		//else
		// LINE: nodeID destID
		sscanf(buf, "%d %d", &u, &v);

		printf("%d %d\n", u, v);
		if (graph) break;
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

	readGraph(F);
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
