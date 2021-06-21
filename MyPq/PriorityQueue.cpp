#include <stdio.h>
#include <stdlib.h>
#include "PriorityQueue.h"

PRIORITYQUEUE * CreatePriorityQueue(int Qsize)
{
	PRIORITYQUEUE *pPQ;
	pPQ = (PRIORITYQUEUE *)malloc(sizeof(PRIORITYQUEUE) + sizeof(int ) * Qsize);
	pPQ->m_pHeap = (int *)&pPQ[1];
	pPQ->m_MaxSize = Qsize;
	pPQ->m_TotalItems = 0;
	return pPQ;
}

void Swap(int *s1, int *s2)
{
	int temp;

	temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

void PrintQueue(PRIORITYQUEUE *pPQ,FILE *pFO)
{
	int i;

	for(i=0;i<pPQ->m_TotalItems;++i)
	{
		fprintf(pFO,"[Element %d] = %d\n",i,pPQ->m_pHeap[i+1]);
	}
}

static int ReheapUp(PRIORITYQUEUE *pPQ)
{
	int child;
	int parent;

    child = pPQ->m_TotalItems;
    while((parent = PARENT(child)) > 0)
	{
		if(pPQ->m_pHeap[child] > pPQ->m_pHeap[parent])
			Swap(&pPQ->m_pHeap[child] ,&pPQ->m_pHeap[parent]);
		child = parent;
	}

	return 0;
}

static int ReheapDown(PRIORITYQUEUE *pPQ)
{
	int parent;
	int child;

	printf("Reheap Down\n");
	for(parent = 1,child = 2;child <= pPQ->m_TotalItems;)
	{
		printf("Parent = %d  Child = %d\n",parent,child);
		//is there a sibling?
		if(child + 1 <= pPQ->m_TotalItems)
		{
			printf("Use Sibling\n");
			if(pPQ->m_pHeap[child+1] > pPQ->m_pHeap[child])
				child += 1;
		}
		if(pPQ->m_pHeap[child] > pPQ->m_pHeap[parent])
			Swap(&pPQ->m_pHeap[child],&pPQ->m_pHeap[parent]);
		parent = child;
		child = CHILD_LEFT(parent);
	}
	return 0;
}

int Insert(PRIORITYQUEUE *pPQ,int item)
{
	pPQ->m_TotalItems++;
	pPQ->m_pHeap[pPQ->m_TotalItems] = item;
	ReheapUp(pPQ);
	PrintQueue(pPQ,stdout);
	return 0;
}

int Delete(PRIORITYQUEUE *pPQ,int *Target)
{
	*Target = pPQ->m_pHeap[1];
	pPQ->m_pHeap[1] = pPQ->m_pHeap[pPQ->m_TotalItems];
	pPQ->m_TotalItems--;
	PrintQueue((pPQ),stdout);
	ReheapDown(pPQ);
	PrintQueue((pPQ),stdout);
	return 0;
}


