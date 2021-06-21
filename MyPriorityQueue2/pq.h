#ifndef PQ_H_INCLUDED
#define PQ_H_INCLUDED

typedef struct {
	void **m_ppHeap;	//pointer to an array of void pointers (the Heap)
	int m_MaxItems;		//maximum number of items in heap
	int m_TotalItems;	//Total number of items in heap
	int (*m_Compare)(void **S1, void **S2);	//pointer to compare function
}PRIORITYQUEUE;

/******************************************************
	m_Compare

	The return value of m_Compare must follow the
	following rules:
	S1 > S2.....return positive value
	S1 = S2.....returns 0
	S1 < S2.....return negative value

******************************************************/

#define PARENT(x)		((x-1)/2)
#define LEFTCHILD(x)	((2 * x) + 1)
#define RIGHTCHILD(x)	((2 * x) + 2)

//-----------------------------
// Function Prototypes
//-----------------------------

void CreatePriorityQueue(PRIORITYQUEUE *pQ,int size,int (*c)(void **,void **));
int Insert(PRIORITYQUEUE *pQ,void *Item);
int Delete(PRIORITYQUEUE *pQ, void **Target);
int Replace(PRIORITYQUEUE *pQ, void *Item, void **Target);

#endif // PQ_H_INCLUDED
