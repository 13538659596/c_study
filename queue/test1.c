#include <stdio.h>
#include <stdlib.h>

#define ElemType char
#define MAXSIZE 10
#define BOOL int
#define FALSE 0
#define TRUE  1

typedef struct{
    ElemType data[MAXSIZE];
    int front, rear;
}Queue;

void InitQueue(Queue *queue);   //初始化队列
BOOL IsEmpty(Queue *queue); //判断是否为空
void EnQueue(Queue *queue, ElemType x); //入队
void DeQueue(Queue *queue, ElemType *x);    //出队
void printQueue(Queue *queue);
int main()
{
    Queue myqueue;
    InitQueue(&myqueue);
    ElemType e;
    printf("%d\n", IsEmpty(&myqueue));
    EnQueue(&myqueue, 'a');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'b');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'c');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'd');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'e');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'f');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'g');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'h');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'i');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'j');
	printQueue(&myqueue);
	printf("###########################\n");
	//------------------------
	EnQueue(&myqueue, 'k');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'l');
	printQueue(&myqueue);
	EnQueue(&myqueue, 'm');
	printQueue(&myqueue);
    return 0;
}

void InitQueue(Queue *queue){
    queue->front = 0;
    queue->rear = 0;
}

void EnQueue(Queue *queue, ElemType e){
    queue->data[queue->rear] = e;
    queue->rear = (queue->rear + 1) % MAXSIZE;
}

void DeQueue(Queue *queue, ElemType *e){
    *e = queue->data[queue->front];
    queue->front = (queue->front + 1) % MAXSIZE;
}

BOOL IsEmpty(Queue *queue){
    if (queue->front == queue->rear) {
        return TRUE;
    } else {
        return FALSE;
    }
}


void printQueue(Queue *queue) 
{

	if(IsEmpty(queue)) {
		printf("队列为空\n");
	}else {
		for(int i = queue->front; i < queue->rear; i++) {
			printf("%6c", queue->data[i]);
		}
		printf("\n");
		printf("--------------------\n");
	}
}


