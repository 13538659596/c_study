#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
/*
* pthread_attr_t  scop detech statckSize policy һ��������⼸������ 
*
*/ 
#define STACK_SIZE 1024* 1024 * 5   //5MB

void * hanle_thread(void *arg);
int main() 
{
		pthread_t threadId; 
		pthread_attr_t attr;
		
		pthread_attr_init(&attr);
		/*
		*int pthread_attr_setscope(pthread_attr_t *attr, int scope);
		*PTHREAD_SCOPE_SYSTEM  ϵͳ�����߳�һ����cpu��Դ 
		*PTHREAD_SCOPE_PROCESS �����������߳̾���cpu��Դ 
		*/
		pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
		
		/*
		*int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
		*PTHREAD_CREATE_DETACHED  ����ģʽ  
		*PTHREAD_CREATE_JOINABLE ����ģʽ     Ĭ������ 
		*/
		int ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		
		printf("ret: %d\n", ret);
		/*
		* int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
		*SCHED_OTHER����������ʵʱ�� Ĭ�� 
		*SCHED_RR��ʵʱ����ת����
		*SCHED_FIFO��ʵʱ�������ȳ��� 
		*/
		pthread_attr_setstacksize(&attr, SCHED_OTHER); 
		
		/*
		*int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
		*
		*/
	//	pthread_attr_setstacksize(&attr, STACK_SIZE);
	
    	pthread_create(&threadId, &attr, hanle_thread, NULL);
    	
    	
    	size_t stacksize;
    	pthread_attr_getstacksize(&attr, &stacksize);
                         
		printf("stacksize: %ld", stacksize);			  
		//	When a thread attributes object is no longer required, it should  be  destroyed  using  the  pthread_attr_destroy()
       //function.  Destroying a thread attributes object has no effect on threads that were created using that object.
	  	pthread_attr_destroy(&attr);
	  	
	  	int *retval;
	  	pthread_join(threadId, (void **)&retval);
	  	//pause();
	  	//printf("retval: %d\n",*retval);
	  	return 0;
} 



void * hanle_thread(void *arg)
{
	int i;
	for(i = 0; i < 10; i++) {
		printf("%d\n", i);
	}
}
