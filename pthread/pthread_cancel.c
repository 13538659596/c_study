#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void * hanle_thread_a(void * arg);
void * hanle_thread_b(void * arg);
int main() 
{
	pthread_t thread_a, thread_b;
	pthread_create(&thread_a, NULL,hanle_thread_a, NULL);
	pthread_create(&thread_b, NULL,hanle_thread_b, (void *)&thread_a);

	pthread_join(thread_a, NULL);
	pthread_join(thread_b, NULL);
	
	return 0;
                          
}

void * hanle_thread_a(void * arg)
{
	 pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	 int i = 0;
	 while(1) {
		printf("i: %d\n", i++);
		sleep(1);
	 }
}

void * hanle_thread_b(void * arg)
{
	int *pid = (int *)arg;
	sleep(3);
	printf("notify thread_a finish\n");
	pthread_cancel(* pid);
}