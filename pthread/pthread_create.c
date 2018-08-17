#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
/*
* pthread_attr_t  scop detech statckSize policy 一般会设置这几种属性 
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
		*PTHREAD_SCOPE_SYSTEM  系统所有线程一起竞争cpu资源 
		*PTHREAD_SCOPE_PROCESS 进程内所有线程竞争cpu资源 
		*/
		pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
		
		/*
		*int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
		*PTHREAD_CREATE_DETACHED  分离模式  
		*PTHREAD_CREATE_JOINABLE 阻塞模式     默认设置 
		*/
		int ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		
		printf("ret: %d\n", ret);
		/*
		* int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
		*SCHED_OTHER（正常、非实时） 默认 
		*SCHED_RR（实时、轮转法）
		*SCHED_FIFO（实时、先入先出） 
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
