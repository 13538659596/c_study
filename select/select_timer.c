#include <stdio.h>
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


void second_sleep(unsigned int sc) ;
void millisecond_sleep(unsigned       long mill) ;
void microsecond_sleep(unsigned       long mic) ;


int main() 
{
	int i;
	for(i = 0; i < 5; i++) {
		printf("i: %d\n",i);
		//second_sleep(1);
		microsecond_sleep(1000 * 500);
	}
	return 0;
}

//秒计时器
void second_sleep(unsigned int sc) 
{
	int ret;
	struct timeval timeout;
	do{
		timeout.tv_sec = sc;
		timeout.tv_usec = 0;
		ret = select(0, NULL, NULL,NULL, &timeout);   //timeout 返回0 
		//printf("ret: %d\n", ret);
	}while(ret);
}

//毫秒计时器
void millisecond_sleep(unsigned       long mill) 
{
	int ret;
	struct timeval timeout;
	do{
		timeout.tv_sec = mill / 1000;
		timeout.tv_usec = (mill % 1000)*1000;
		ret = select(0, NULL, NULL,NULL, &timeout);   //timeout 返回0 
		//printf("ret: %d\n", ret);
	}while(ret);
}

//微秒计时器
void microsecond_sleep(unsigned       long mic) 
{
	int ret;
	struct timeval timeout;
	do{
		timeout.tv_sec = mic / (1000 * 1000);
		timeout.tv_usec = mic % (1000*1000);
		ret = select(0, NULL, NULL,NULL, &timeout);   //timeout 返回0 
		//printf("ret: %d\n", ret);
	}while(ret);
}


