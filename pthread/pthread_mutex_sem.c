#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/shm1"
#define SHM_OFLAG O_RDWR | O_CREAT
#define SHM_MODE 00600
#define MMAP_PROT PROT_WRITE | PROT_READ
#define MMAP_FLAGES MAP_SHARED
#define SHM_LEN 1024
#define MSG_LEN 1024

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void * thread_p1(void *) ;
void * thread_p2(void *) ;
/*
*
*		
*
*鍙戠敓杈惧埌鎾掓棪
*/
int main() 
{
	shm_unlink(SHM_NAME);
	int fd = shm_open(SHM_NAME, SHM_OFLAG, SHM_MODE);
	if(fd  == -1) {
		perror("shm_open");
		return -1;
	}
	
	//设置共享内存的大小
     int ret = ftruncate(fd, SHM_LEN);
	if(ret == -1) {
		perror("ftruncate");
		return -1;
	}

	//将共享内存映射到当前使用的内存中
	void * mmap_buff = mmap(NULL, SHM_LEN, MMAP_PROT, MMAP_FLAGES,
                   fd, 0L);
	if(mmap_buff == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
	memset(mmap_buff, 0, SHM_LEN);

	//创建线程
	pthread_t p1, p2;
	p1 = pthread_create(&p1, NULL, thread_p1,mmap_buff);
	p2 = pthread_create(&p2, NULL, thread_p2, mmap_buff);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	printf("000000000000\n");
	//关闭文件
	close(fd);
	
	//取消映射内存
	munmap(mmap_buff, SHM_LEN);
	return 0;
}

void * thread_p1(void *arg) 
{
	//拿到共享内
	char *mmap_nuf = (char *)arg;
	char buff[MSG_LEN + 1]= {0};
	printf("请输入文件名\n");
	fgets(buff, MSG_LEN, stdin);
	ssize_t len = strlen(buff);
	pthread_mutex_lock(&mutex1);
	memcpy(mmap_nuf, buff, len);
	pthread_mutex_unlock(&mutex2);
	while(1);
}

void * thread_p2(void *arg) 
{
	char *mmap_nuf = (char *)arg;
	char buff[MSG_LEN + 1]= {0};
	printf("thread_p2 获取锁\n");
	pthread_mutex_lock(&mutex2);
	printf("thread_p2 获取锁成功\n");
	memcpy(buff, mmap_nuf, strlen(mmap_nuf));
	printf("文件名称是: %s\n", buff);
}


