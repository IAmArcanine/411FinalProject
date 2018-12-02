#include "pgRnk.h"

COINFACE tossWeightedCoin(double dampingVal){
	//generate a number between 0 and 1
	//
	double coinflip = (double)rand() / (double)((unsigned)RAND_MAX + 1);
	return (coinflip <= dampingVal) ? HEADS : TAILS;
}

void printGraph(Graph *g){
	int i = 0;
	for (i = 0; i < g->numNodes; i++){
		printf("( %d : ", g->nodeArray[i].id + 1);
		printList(&(g->nodeArray[i]));
		printf(" )\n");
	}
}

void readGraph(Graph *graphObject, char *filename){
	//printf("DEBUG: readGraphStart\n");
	FILE *infile = NULL;
	char buf[50];
	int u, v;
	int n = 0;

	infile = fopen(filename, "r");

	//step 1: determine then number of nodes.
	

	//read each line
	if (!infile){ //file failed to open
		printf("ERROR: file open failed.\n");
		exit(1);
	}
	/*else {
		//if the graph is ordered, read the last line, allocate memory once, and read the file
		
		fseek(infile, -50, SEEK_END);
		fgets(buf, sizeof(buf), infile);
		fseek(infile, 0, 0);
		sscanf(buf, "%d %d", &u, &v);
		graphObject->nodeArray = (List *) malloc((u+1) * sizeof(List));
		if (!graphObject->nodeArray) {
			printf("calloc returned null\n");
			return;
		}
		graphObject->numNodes = u+1;
	}************************************************************
		This doesn't work when the max node never shows up as u
	************************************************************/
	//printf("initmalloc\n");
	graphObject->nodeArray = (List *) malloc(sizeof(List));
	graphObject->numNodes = 1;
	graphObject->nodeArray[0].id = 0;
	graphObject->nodeArray[0].size = 0;
	graphObject->nodeArray[0].pHead = NULL;
	//printf("while loop\n");
	while (!feof(infile)){
		if (!fgets (buf, sizeof(buf), infile)) { break; }//if null just end
		//else
		if (buf[0] == '#') continue;
		// LINE: nodeID destID
		sscanf(buf, "%d %d", &u, &v);
		if (u >= v){
			if (n-1 < u) n = u+1;
		}
		else{
			if (n-1 < v) n = v+1;
		}

		//printf("%d %d\n", u, v);
		if (n > graphObject->numNodes){ //need to resize array
			//printf("n[%d]\n", n);
			graphObject->nodeArray = (List *) realloc(graphObject->nodeArray, sizeof(List) * (n));
			//zero the memory out there
			memset(&graphObject->nodeArray[graphObject->numNodes], 0, sizeof(List)*((n)-graphObject->numNodes));
			graphObject->numNodes = n;
		}
		insertFront(&graphObject->nodeArray[u], v);// insert this edge into the array
		graphObject->nodeArray[u].id = u;

	}
	fclose(infile);
	for (v = 1; v < n; v++) { //dead end nodes don't have an ID
		if (graphObject->nodeArray[v].id == 0) {
			graphObject->nodeArray[v].id = v;
		}
	}
	//printGraph(graphObject);
	
	//printf("DEBUG: readGraph end\n");
}

void printUsage(){
	printf("Usage - ");
	printf("pagerank {K: length of random walk} {D: damping ratio} {F: graph file}\n");
}

void findMax5(int *arr, int size){
	//printf("DEBUG: FindMax5 Start");
	int i;
	int top[5] = {-1,-1,-1,-1,-1};
	for (i = 0; i < size; i++){
		if (arr[top[0]] <= arr[i]){
			top[4] = top[3];
			top[3] = top[2];
			top[2] = top[1];
			top[1] = top[0];
			top[0] = i;
		}
	}
	printf("Top 5: \t[%d]: %d\t[%d]: %d\t[%d]: %d\t[%d]: %d\t[%d]: %d\n", 
			top[0], arr[top[0]],
			top[1], arr[top[1]],
			top[2], arr[top[2]],
			top[3], arr[top[3]],
			top[4], arr[top[4]]);
	//printf("DEBUG: FindMax5 End");
}
