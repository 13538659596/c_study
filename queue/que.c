#include "que.h"

void init(Q* que) 
{	
	que->first = 0;
	que->end = 0;
}


int isEmpty(Q* que) 
{
	return que->first == que->end;
}

void enQue(Q* que, DATA_TYPE msg)
{
	if(que->end == NUMBER) {
		printf("队列已满\n");
	}else {
		que->data[que->end] = msg;
		que->end++;
	}
}

DATA_TYPE popQue(Q* que)
{
	if(isEmpty(que)) {
		printf("队列为空\n");
	}else {
		que->first++;
		que->end = (que->end++)
		return que->data[que->first - 1];
		
		
	}
}

void printQue(Q* que)
{
	if(isEmpty(que)) {
		printf("队列为空\n");
	}else {
		for(int i = que->first; i < que->end; i++) {
			printf("%6c", que->data[i]);
		}
		printf("\n");
		printf("--------------------\n");
	}
	
}

DATA_TYPE getHead(Q* que) 
{
	if(isEmpty(que)) {
		printf("队列为空\n");
	}else {
		return que->data[que->first];
	}
	
}
	





