#include <stdio.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <string.h>
#include <pthread.h>


#define SERVER_NAME "./socket"
#define MSG_LEN 1024
//客户端
void * my_rec(void * arg); 
void * my_send(void * arg);
int main() 
{	
	//创建本地socket
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd == -1) {
		perror("socket");
		return -1;
	}
	struct sockaddr_un addr;
	ssize_t len = sizeof(addr);
	memset(&addr, 0, len);
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, SERVER_NAME,strlen(SERVER_NAME));
	int serfd = connect(sockfd, (struct sockaddr *)&addr, len);
	if(serfd == -1) {
		perror("connect");
		close(sockfd);
		return -1;
	}
	
	//创建两个线程。1个读数据，一个写数据
	pthread_t p1, p2;
	pthread_create(&p1, NULL,my_rec, &sockfd);
    pthread_create(&p2, NULL,my_send, &sockfd);                      

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	
	close(sockfd);
	return 0;
}

void * my_rec(void * arg) 
{
	int * sockfd = (int *)arg;
	
	char buff[MSG_LEN + 1] = {0};
	ssize_t len;
	while (1){
		memset(&buff, 0, MSG_LEN);
		len = read(*sockfd, buff, MSG_LEN);
		if(len <= 0) {
			printf("sockfd is closed:\n");
			break;
		}
		printf("recv msg: %s\n", buff);
		if(strstr(buff, "end")) {
			break;
		}
	}
	pthread_exit(NULL);	
}

void * my_send(void * arg) 
{
	int * sockfd = (int *)arg;
	char buff[MSG_LEN + 1] = {0};
	ssize_t len;
	printf("请输入数据\n");
	while (1){
		memset(&buff, 0, MSG_LEN);
		fgets(buff, MSG_LEN, stdin);
		len = strlen(buff);
		if(buff[len - 1] == '\n') {
			buff[len - 1] = '\0';
		}
		printf("写入的数据是: %s\n", buff);
		len =  write(*sockfd, buff, strlen(buff));
		printf("write len: %ld\n", len);
		if(strstr(buff, "end")) {
			break;
		}
	}
	pthread_exit(NULL);
}
