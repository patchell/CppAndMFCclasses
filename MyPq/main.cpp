#include <stdio.h>
#include "PriorityQueue.h"

void PrintSeparator(FILE *pO)
{
	fprintf(pO,"--------------------\n");
}

int main()
{
	int item;

	PRIORITYQUEUE *pQ;
	pQ = CreatePriorityQueue(20);
	Insert(pQ,10);
	PrintSeparator(stdout);
	Insert(pQ,11);
 	PrintSeparator(stdout);
	Insert(pQ,12);
 	PrintSeparator(stdout);
	Insert(pQ,9);
	Insert(pQ,1);
	Insert(pQ,2);
	Insert(pQ,3);
	Insert(pQ,4);
	Insert(pQ,5);
	Insert(pQ,6);
	while(pQ->m_TotalItems)
	{
		PrintSeparator(stdout);
		Delete(pQ,&item);
		printf("Retrieved Item %d\n",item);
	}
    return 0;
}
