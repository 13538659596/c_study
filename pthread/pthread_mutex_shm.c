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
#define FILE_FLAGS O_RDONLY
#define SHM_LEN 1024
#define MSG_LEN 1024


//两个锁用来确定线程的执行先后顺序
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void * thread_p1(void *) ;
void * thread_p2(void *) ;

/*
*1线程往共享内存中写入文件名，2线程读取文件名打开后读取文件内容写入共享内存
*1线程将共享内存里的内容读出来输出到屏幕上
*/
int main() 
{
	shm_unlink(SHM_NAME);
	int fd = shm_open(SHM_NAME, SHM_OFLAG, SHM_MODE);
	if(fd  == -1) {
		perror("shm_open");
		return -1;
	}
	
	
    int ret = ftruncate(fd, SHM_LEN);
	if(ret == -1) {
		perror("ftruncate");
		return -1;
	}

	//映射共享内存
	void * mmap_buff = mmap(NULL, SHM_LEN, MMAP_PROT, MMAP_FLAGES,
                   fd, 0L);
	if(mmap_buff == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
	memset(mmap_buff, 0, SHM_LEN);
	//锁住，不让线程2获取，直到线程1输入文件名才释放
	pthread_mutex_lock(&mutex2);
	pthread_t p1, p2;
	pthread_create(&p1, NULL, thread_p1,mmap_buff);
	pthread_create(&p2, NULL, thread_p2, mmap_buff);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	
	//关闭文件
	close(fd);
	
	//取消共享内存映射
	munmap(mmap_buff, SHM_LEN);
	return 0;
}

void * thread_p1(void *arg) 
{
	char *mmap_buf = (char *)arg;
	char buff[MSG_LEN + 1]= {0};
	printf("请输入文件名\n");
	
	//fgets会保存换行符'\n'
	fgets(buff, MSG_LEN, stdin);
	ssize_t len = strlen(buff);
	pthread_mutex_lock(&mutex1);
	memcpy(mmap_buf, buff, len - 1);
	pthread_mutex_unlock(&mutex2);
	while(1){
		pthread_mutex_lock(&mutex1);
		printf("thread_p1 获取锁\n");
		memset(buff, 0, MSG_LEN);
		memcpy(buff, mmap_buf, MSG_LEN);
		len = strlen(buff);
		if(len > 0) {
			printf("thread_p1 读取共享内存的内容\n");
			write(STDOUT_FILENO, buff, MSG_LEN);
			fflush(stdout);
			memset(mmap_buf, 0, MSG_LEN);
		}else {
			pthread_mutex_unlock(&mutex2);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&mutex2);
	}
}

void * thread_p2(void *arg) 
{
	char *mmap_buf = (char *)arg;
	char buff[MSG_LEN + 1]= {0};
	//阻塞，直到输入文件名后可以线程1解锁
	pthread_mutex_lock(&mutex2);
	printf("thread_p2 获取锁成功  %ld\n", strlen(mmap_buf));
	memcpy(buff, mmap_buf, strlen(mmap_buf));
	printf("文件名是 : %s\n", buff);
	int fd = open(buff, FILE_FLAGS);
	if(fd  == -1) {
		perror("open");
		pthread_mutex_unlock(&mutex1);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&mutex2);
	printf("文件打开成功\n");
	while(1) {
		pthread_mutex_lock(&mutex2);
		memset(buff, 0, MSG_LEN);
		ssize_t len = read(fd, buff, MSG_LEN);
		if(len > 0) {
			memcpy(mmap_buf, buff, MSG_LEN);
		}else {
			pthread_mutex_unlock(&mutex1);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&mutex1);
	}
	
}



