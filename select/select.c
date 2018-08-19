#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define FILE_NAME "/dev/pts/8"
#define FILE_FALGS O_RDONLY
int main() 
{
	char ch;
	int fd = open(FILE_NAME, FILE_FALGS);
	if(fd == -1) {
		perror("open");
		return -1;
	}
	int nfds = fd + 1;
	fd_set readfds;
	

	struct timeval timeout;
	while(1) {
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;
	
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
	
		int ret = select(nfds , &readfds, NULL,NULL, &timeout);
		if(ret == -1) {
			perror("select");
			break;
		}
       if(FD_ISSET(fd, &readfds)) {
	   		//printf("有数据\n");
			ssize_t len = read(fd, &ch, sizeof(char));
		 if(ch == 'q') {
			break;
		 }else if(ch == '\n') {
			continue;
		 }else {
			printf("%c\n",ch);
		 }
       }else {
			 //printf("timeout\n");
       	}
	}
	
	return 0;
}
