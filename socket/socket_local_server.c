#include <stdio.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <string.h>
#include <pthread.h>


#define SERVER_NAME "./socket"
#define MSG_LEN 1024
#define LISTEN_BACKLOG 3
//服务端


int main() 
{	
	unlink(SERVER_NAME);
	//创建本地socket
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd == -1) {
		perror("socket");
		unlink(SERVER_NAME);
		close(sockfd);
		return -1;
	}
	struct sockaddr_un addr;
	ssize_t len = sizeof(addr);
	memset(&addr, 0, len);
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, SERVER_NAME,strlen(SERVER_NAME));
	
	int ret = bind(sockfd, (struct sockaddr *)&addr,sizeof(addr));
	if(ret == -1) {
		perror("bind");
		unlink(SERVER_NAME);
		close(sockfd);
		return -1;
	}
	if (listen(sockfd, LISTEN_BACKLOG) == -1) {
		perror("listen");
		unlink(SERVER_NAME);
		close(sockfd);
		return -1;

	}	
	struct sockaddr_un client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	socklen_t client_addr_len = sizeof(client_addr);
	int cfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_len);
	if(cfd == -1) {
		perror("accept");
		unlink(SERVER_NAME);
		close(sockfd);
		return -1;
	}
	printf("客户端已经连接  : %s\n", client_addr.sun_path);
	char buff[MSG_LEN + 1] = {0};
	ssize_t msg_len;
	while(1) {
		memset(&buff, 0, MSG_LEN);
		msg_len = read(cfd, buff, sizeof(buff));
		if(msg_len <= 0) {
			printf("客户端socket已经关闭\n");
			break;
		}
		if(msg_len > 0){
			printf("客户端发来发来的消息是: %s\n", buff);
			write(cfd, buff,len);
			if(strstr(buff, "end")) {
				break;
			}
		}
		
	}

	close(sockfd);
	close(cfd);
	unlink(SERVER_NAME);

	return 0;
                       
}
