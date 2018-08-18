#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void * add1(void * arg) ;
void * add2(void * arg) ;

typedef struct ct_sum {
	int sum;
	pthread_mutex_t lock;

}ct_sum;
int main() 
{
	pthread_t p1, p2;

	ct_sum cnt;
	pthread_mutex_init(&(cnt.lock), NULL);
	cnt.sum = 100;
	
	pthread_create(&p1, NULL, add1,  &cnt);
	pthread_create(&p2, NULL, add2,  &cnt);

	//pthread_mutex_lock(&(cnt.lock));
	pthread_mutex_unlock(&(cnt.lock));
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	//pthread_mutex_destroy(&(cnt.lock));
	return 0;
                         
}

void * add1(void * arg) 
{
	ct_sum * sum = (ct_sum *)arg;

	
	while (1) {
		pthread_mutex_lock(&(sum->lock));
		if(sum->sum > 0) {
			sum->sum--;
			printf("add1 sum: %d\n", sum->sum);
		}
		pthread_mutex_unlock(&(sum->lock));
	}
	
	pthread_exit(NULL);
}

void * add2(void * arg) 
{
	int i;
	ct_sum * sum = (ct_sum *)arg;
	
	while (1) {
		pthread_mutex_lock(&(sum->lock));
		if(sum->sum > 0) {
			usleep(200*1000);
			sum->sum--;
			printf("add2 sum: %d\n", sum->sum);
		}
		pthread_mutex_unlock(&(sum->lock));
	}
	
	pthread_exit(NULL);
}

