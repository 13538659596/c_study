#include <stdio.h>
#define NUMBER 10
#define DATA_TYPE char

struct queue 
{
	DATA_TYPE data[NUMBER];  //保存数据的数组
	int first;  //数据的开头
	int end;    //数据的结尾
	
};
typedef struct queue Q;

/**
*初始化队列
*/


void init(Q* que);
/**
*判断队列是否为空
*/
int isEmpty(Q* que);

/**
*入队
*/
void enQue(Q* que, DATA_TYPE msg);

/**
*出队
*/
DATA_TYPE popQue(Q* que);

/**
*打印队列所有元素
*/
void printQue(Q* que);


/**
*获取开头的一个节点
*/
DATA_TYPE getHead(Q* que);



