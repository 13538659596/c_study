#include <stdio.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_DOMAIN AF_INET
#define SOCKE_TYPE SOCK_STREAM
#define SOCKE_PORT 9990
#define SOCKE_LITSEN_BACKLOG 5
#define MSG_LEN 1024


//	sockaddr_in（在netinet/in.h中定义）：
//	struct sockaddr_in {
//	short int sin_family;               /* Address family */
//	unsigned short int sin_port;       /* Port number */
//	struct in_addr sin_addr;            /* Internet address */
//	unsigned char sin_zero[8];         /* Same size as struct sockaddr */
//	};

void handle_cilent(int sockfd) ;

int main() 
{
	int sockfd = socket(SOCKET_DOMAIN, SOCKE_TYPE, 0);
	printf("befor sockfd: %d\n", sockfd);
	if(sockfd == -1) {
		perror("socket");
		return -1;
	}
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = SOCKET_DOMAIN;
	addr.sin_port = htons(SOCKE_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);    //监听本机所有网卡
	
	int ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	if(ret == -1) {
		perror("bind");
		return -1;
	}

	ret = listen(sockfd, SOCKE_LITSEN_BACKLOG);
	
	if(ret == -1) {
		perror("listen");
		return -1;
	}

	printf("等待客户端的连接\n");
	//memset(&addr, 0, sizeof(addr));

	struct sockaddr_in cliaddr;
	int addrlen = sizeof(cliaddr);
	printf("after sockfd: %d\n", sockfd);

	int client = accept(sockfd, (struct sockaddr *)&cliaddr, &addrlen);
	
	if(client == -1) {
		perror("bind");
		return -1;
	}
	printf("发现客户端连接: %s\n",inet_ntoa(cliaddr.sin_addr));
	handle_cilent(client);
                
	
}

void handle_cilent(int sockfd) 
{
	char buf[MSG_LEN];
	while(1) {
		 ssize_t ret = read(sockfd, buf, MSG_LEN);
		 if(ret == -1) {
			perror("read");
			break;
		 }else if(ret == 0) {
			 break;
		 }
		 buf[ret] = '\0';
		 int i;
		 for (i = 0; i < ret; i++) {
			buf[i] = buf[i] + 'A' - 'a';
		 }
		 
		printf("%s\n", buf);
		write(sockfd, buf, ret);
				 
		if(strncmp(buf, "end", 3) == 0)
		{
			break;
		}
	}
}
