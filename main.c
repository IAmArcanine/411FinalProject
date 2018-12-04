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
	int p = 1;

	srand(time(NULL));
	if (argc != 5){ //incorrect number of input arguments
		printUsage();
		exit(1);
	}

	K = atoi(argv[1]);
	D = atof(argv[2]);
	F = strdup(argv[3]);
	p = atoi(argv[4]);
	//int rank = omp_get_thread_num();

	//printf("Rank: %d, #threads: %d\n", rank, thread_count);
	

	readGraph(&g, F);
	//created an array where arr[NODE_ID] = TIMES_VISITED
	//NOTE: threads will need to aquire locks to write to this memory
	visited = (int*)calloc(g.numNodes, sizeof(int));

	//printGraph(&g); //for testing


	//printf("DEBUG: starting main loop with g.numNodes[%d]\n", g.numNodes);
	omp_lock_t visitedLock;
	omp_init_lock(&visitedLock);
	double t = omp_get_wtime();
	int temp;
	int rank = -1;
	#pragma omp parallel for num_threads(p) private(step)// schedule(guided)
	for (i = 0; i < g.numNodes; i++) {  //starting at every node in the graph
		if (omp_get_thread_num() > rank){
			temp = omp_get_thread_num();
			#pragma omp critical
			rank = temp;
		}
		///NOTE: the conceptual graph node is represented by a List object
		//printf("\n");
		//printf("\tDEBUG: main loop i[%d] \n", i);
		List *pCur = &g.nodeArray[i]; //curr <- node i
		for (step = 1; step <= K; step++){
			//printf("\t\tDEBUG: step loop step[%d] \n", step);
			//CRITICAL
			//omp_set_lock(&visitedLock);
			#pragma omp atomic
			visited[pCur->id]++;
			//omp_unset_lock(&visitedLock);
			//printf("%d -> ",pCur->id);
			//CRITICAL END
			
			//toss a coin
			if (tossWeightedCoin(D) == HEADS){
				//successor = pick a random node between 1 and n (each with prob 1/n)
				//printf("\t\tHEADS: choose random\n");
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
					//printf("\t\t\tTAILS: deadEnd\n");
					pCur = &g.nodeArray[(rand()%g.numNodes)];
				}
			}
		}
	}
	omp_destroy_lock(&visitedLock);
	findMax5(visited, g.numNodes);
	t = omp_get_wtime() - t;

	printf("Using % 3d thread(s) with %d nodes, %d steps per walk, dampingVal[%f]: %f\n",
		rank+1, g.numNodes, K, D, t);
	//getchar();
}


