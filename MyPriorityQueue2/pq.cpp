#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

void CreatePriorityQueue(PRIORITYQUEUE *pQ,int size,int (*c)(void **,void **))
{
	pQ->m_ppHeap = (void **)malloc(sizeof(void *) * size);
	pQ->m_MaxItems = size;
	pQ->m_TotalItems = 0;
	pQ->m_Compare = c;
}

static void Swap(void **ppS1,void **ppS2)
{
	void *pTemp;
	pTemp = *ppS1;
	*ppS1 = *ppS2;
	*ppS2 = pTemp;
}

static void ReheapUp(PRIORITYQUEUE *pQ)
{
	int child;
	int parent;
	int loop;

    child = pQ->m_TotalItems - 1;
    while((parent = PARENT(child) ) >= 0)
	{
		printf("Parent:%d Child:%d\n",parent,child);
		if((*pQ->m_Compare)(&pQ->m_ppHeap[parent],&pQ->m_ppHeap[child]) >= 0)
			break;
		Swap(&pQ->m_ppHeap[child] ,&pQ->m_ppHeap[parent]);
		child = parent;
	}

}

static void ReheapDown(PRIORITYQUEUE *pQ)
{
	int parent;
	int child;

	for(parent = 0,child = 1;child < pQ->m_TotalItems;)
	{
		//is there a sibling?
		if(child + 1 < pQ->m_TotalItems)
		{
			if((*pQ->m_Compare)( &pQ->m_ppHeap[child] ,& pQ->m_ppHeap[child+1]) > 0)
				child += 1;	//use the sibling
		}
		if((*pQ->m_Compare)(&pQ->m_ppHeap[parent] , &pQ->m_ppHeap[child])> 0)
			break;
		Swap(&pQ->m_ppHeap[parent],&pQ->m_ppHeap[child]);
		parent = child;
		child = LEFTCHILD(parent);
	}
}

int Insert(PRIORITYQUEUE *pQ,void *Item)
{
	if(pQ->m_TotalItems < pQ->m_MaxItems)
	{
		pQ->m_ppHeap[pQ->m_TotalItems++] = Item;
		ReheapUp(pQ);
	}
	return pQ->m_MaxItems - pQ->m_TotalItems;
}

int Delete(PRIORITYQUEUE *pQ, void **Target)
{
	if(pQ->m_TotalItems)
	{
		*Target = pQ->m_ppHeap[0];
		pQ->m_ppHeap[0] = pQ->m_ppHeap[--pQ->m_TotalItems];
		ReheapDown(pQ);
	}
	return pQ->m_TotalItems;
}

int Replace(PRIORITYQUEUE *pQ, void *Item, void **Target)
{
	if(pQ->m_TotalItems)
	{
		*Target = pQ->m_ppHeap[0];
		pQ->m_ppHeap[0] = Item;
		ReheapDown(pQ);
	}
	return pQ->m_MaxItems - pQ->m_TotalItems;
}
