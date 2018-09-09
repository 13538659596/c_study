
#include "que.h"

int main() 
{
	Q myqueue;
	init(&myqueue);
	enQue(&myqueue, 'a');
	printQue(&myqueue);
	enQue(&myqueue, 'b');
	printQue(&myqueue);
	enQue(&myqueue, 'c');
	printQue(&myqueue);
	enQue(&myqueue, 'd');
	printQue(&myqueue);
	enQue(&myqueue, 'e');
	printQue(&myqueue);
	enQue(&myqueue, 'f');
	printQue(&myqueue);
	enQue(&myqueue, 'g');
	printQue(&myqueue);
	enQue(&myqueue, 'h');
	printQue(&myqueue);
	enQue(&myqueue, 'i');
	printQue(&myqueue);
	enQue(&myqueue, 'j');
	printQue(&myqueue);
	printf("###########################\n");
	//------------------------
	enQue(&myqueue, 'k');
	printQue(&myqueue);
	enQue(&myqueue, 'l');
	printQue(&myqueue);
	enQue(&myqueue, 'm');
	printQue(&myqueue);
	enQue(&myqueue, 'n');
	popQue(&myqueue);
	enQue(&myqueue, 'k');
	printQue(&myqueue);
	return 0;
}
