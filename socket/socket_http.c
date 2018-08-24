#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <semaphore.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define PROTOCOL_FAMILY AF_INET
#define PORT 10300
#define LISTEN_BACKLOGS 5
#define MSG_LEN 1024


void * handle_client(void *) ;
sem_t sem;

struct client_info{
	int *clientfd;
	struct sockaddr_in *clent_addr;
};

int main() 
{
	int ret;
	struct client_info clientInfo;
	int sockfd = socket(PROTOCOL_FAMILY, SOCK_STREAM, 0);
	if( sockfd == -1) {
		perror("sockfd");
		return -1;
	}
	struct sockaddr_in addr;
	ssize_t addr_len = sizeof(struct sockaddr_in);
	memset(&addr, 0, addr_len);
	addr.sin_family = PROTOCOL_FAMILY;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	ret = bind(sockfd, (struct sockaddr*)&addr,addr_len);
	if(ret == -1) {
		perror("bind");
		close(sockfd);
		return -1;
	}
                
	ret = listen(sockfd, LISTEN_BACKLOGS);
	if(ret == -1) {
		perror("listen");
		close(sockfd);
		return -1;
	}
	
	//use sem
	sem_init(&sem, 1, 1);
	
	//resive client connect
	int clientfd = {0};
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(struct sockaddr_in);
	while(1) {
		//lock
		sem_wait(&sem);
		memset(&client_addr, 0, client_addr_len);
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
		
		if(clientfd == -1) {
			perror("accept");
			continue;
		}  
		printf("a new client: %d[%s : %d] connect.\n ", clientfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	
		memset(&clientInfo, 0, sizeof(clientInfo));
		clientInfo.clientfd = &clientfd;
		clientInfo.clent_addr = &client_addr;
		//create pthread handle client request
		//pthread_t p_client;
	//	pthread_create(&p_client, NULL,handle_client, &clientInfo);
	//	pthread_detach(p_client);

        handle_client(&clientInfo);            
	}
	
	
	close(sockfd);
	sem_destroy(&sem);
	return 0;	
} 


void * handle_client(void *arg) 
{

	struct client_info *client = (struct client_info *)arg;
	int client_fd = *client->clientfd;
	struct sockaddr_in client_addr = *client->clent_addr;
	
	//unlock
	sem_post(&sem);
	
	char buff[MSG_LEN + 1] = {0};
	ssize_t len;
	//
	pid_t pid = fork();
	if(pid < 0) {
		perror(" fork");
		return NULL;
	}else if(pid == 0){
		len = recv(client_fd, buff, MSG_LEN, 0);
		if(len <= 0) {
			//client close
			exit(0);
		}
		printf("%d[%s : %d] %s\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buff);

		//paser data
		char file_name[MSG_LEN + 1] = {0};
		int sscanf(buff, "GET /%[^ ]",  file_name);
		printf("file_name %s", file_name);

	}else {
		close(client_fd);
		wait(NULL);
		printf("-----------------------\n");
	}
}
