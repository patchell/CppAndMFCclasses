#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

PRIORITYQUEUE q;
typedef struct {
	int value;
}ITEM;

int Compare(void **ppS1, void **ppS2)
{
	ITEM *pItem1 = (ITEM *)*ppS1;
	ITEM *pItem2 = (ITEM *)*ppS2;
	return pItem1->value - pItem2->value;
}

ITEM *CreateItem(int v)
{
	ITEM *pI = (ITEM *)malloc(sizeof(ITEM));
	pI->value = v;
	return pI;
}

void PrintQueue(PRIORITYQUEUE *pQ)
{
	int i;
	ITEM *pI;

	for(i=0;i<pQ->m_TotalItems;++i)
	{
		pI = (ITEM *)pQ->m_ppHeap[i];
		printf("%d:Value:%d\n",i,pI->value);
	}
	printf("========================\n");
}

int main()
{
	ITEM *Targ,*Item;

	CreatePriorityQueue(&q,20,Compare);
	Insert(&q,CreateItem(19));
	PrintQueue(&q);
	Insert(&q,CreateItem(10));
	PrintQueue(&q);
	Insert(&q,CreateItem(20));
	PrintQueue(&q);
	Insert(&q,CreateItem(14));
	PrintQueue(&q);
	Insert(&q,CreateItem(13));
	PrintQueue(&q);
	Insert(&q,CreateItem(16));
	PrintQueue(&q);
	Insert(&q,CreateItem(15));
	PrintQueue(&q);
	Insert(&q,CreateItem(1));
	PrintQueue(&q);
	Insert(&q,CreateItem(25));
	PrintQueue(&q);
	Insert(&q,CreateItem(4));
	Item = CreateItem(12);
	Replace(&q,(void *)Item,(void **)&Targ);
	PrintQueue(&q);
	while(q.m_TotalItems)
	{
		Delete(&q,(void **)&Targ);
		printf("Removed %d\n",Targ->value);
	}
    return 0;
}
