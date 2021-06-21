#ifndef PRIORITYQUEUE_H_INCLUDED
#define PRIORITYQUEUE_H_INCLUDED

#define CHILD_LEFT(x)		(x * 2)
#define CHILD_RIGHT(x)		((x*2)+1)
#define PARENT(x)			(x/2)

typedef struct {
	int *m_pHeap;
	int m_MaxSize;
	int m_TotalItems;
}PRIORITYQUEUE;

PRIORITYQUEUE * CreatePriorityQueue(int size);
int Insert(PRIORITYQUEUE *pPQ,int item);
int Delete(PRIORITYQUEUE *pPQ,int *Target);

#endif // PRIORITYQUEUE_H_INCLUDED
