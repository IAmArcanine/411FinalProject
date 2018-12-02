// Matthew Ferran
//
#include "list.h"
#include "pgRnk.h"

int main(int argc, char *argv[]){
	int K = 0;
	double D =  0.0;
	char *F;
	int i = 0, step = 0;
	Graph g;
	int *visited = NULL;

	srand(time(NULL));

#ifdef _OPENMP
	int rank = omp_get_thread_num();
	int thread_count = omp_get_num_threads();
#else
	int rank = 0;
	int thread_count = 1;
#endif
	if (argc != 4){ //incorrect number of input arguments
		printUsage();
		printf("Rank: %d, #threads: %d\n", rank, thread_count);
		exit(1);
	}

	K = atoi(argv[1]);
	D = atof(argv[2]);
	F = strdup(argv[3]);

	readGraph(&g, F);
	//created an array where arr[NODE_ID] = TIMES_VISITED
	//NOTE: threads will need to aquire locks to write to this memory
	visited = (int*)calloc(g.numNodes, sizeof(int));

	//printGraph(&g); //for testing


	printf("DEBUG: starting main loop with g.numNodes[%d]\n", g.numNodes);
	for (i = 0; i < g.numNodes; i++) {  //starting at every node in the graph
		///NOTE: the conceptual graph node is represented by a List object
		//printf("\n");
		printf("\tDEBUG: main loop i[%d] \n", i);
		List *pCur = &g.nodeArray[i]; //curr <- node i
		for (step = 1; step <= K; step++){
			printf("\t\tDEBUG: step loop step[%d] \n", step);
			//CRITICAL
			visited[pCur->id]++;
			//printf("%d -> ",pCur->id);
			//CRITICAL END
			
			//toss a coin
			if (tossWeightedCoin(D) == HEADS){
				//successor = pick a random node between 1 and n (each with prob 1/n)
				printf("\t\tHEADS: choose random\n");
				pCur = &g.nodeArray[(rand()%g.numNodes)];
				//curr <- succ;
			}
			else {// w/prob 1-D
				//curr <- pick a random node from one of curr's out links
				if (pCur->size){
					//printf("\t\t\tTAILS: choose nearby\n");
					pCur = &g.nodeArray[getDestId(pCur, rand()%pCur->size)];
				}
				else{
					printf("\t\t\tTAILS: deadEnd\n");
					pCur = &g.nodeArray[(rand()%g.numNodes)];
				}
			}
		}
	}
	findMax5(visited, g.numNodes);
}


