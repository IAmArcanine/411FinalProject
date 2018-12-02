#include "list.h"

//simply allocate a new ListNode on the heap
Node *makeNode(int data){
	Node *pNew = (Node*)malloc(sizeof(Node));

	pNew->id = data;
	pNew->pNext = NULL;

	return pNew;
}

//create a new Node and insert it at the front of the List. the List size is # of outgoing links (m)
void insertFront(List *pL, int data){
	Node *pNew = makeNode(data);

	pNew->pNext = pL->pHead;
	pL->pHead = pNew;
	pL->size++;

	return;
}

int getDestId(List *pL, int randM){
	Node *pCur = pL->pHead;
	int i;

	for (i = 0; i < randM; i++){
		pCur = pCur->pNext;
	}

	//return the id of the other end of the edge
	return pCur->id;
}
void printList(List *pL){
	Node *pCur = pL->pHead;

	while(pCur){
		printf("%d, ",pCur->id);
		pCur = pCur->pNext;
	}
}
List * makeList(){
	return (List*)calloc(1, sizeof(List));
}
