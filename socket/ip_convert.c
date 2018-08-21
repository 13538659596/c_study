#include <stdio.h>
#include <arpa/inet.h>

#define IP_LEN INET_ADDRSTRLEN
int main() 
{
	char ip[] = "192.168.1.255";

	int dst; 
	//通过这个方法的返回值可以判断ip是否合法，返回结果为0则不合法
	
	int ret = inet_pton(AF_INET, ip, &dst);

	printf("pton  ret: %d  dst: %d\n", ret, dst);

	char myip [IP_LEN] = {0};
	inet_ntop(AF_INET, &dst,myip, IP_LEN);
								
	
	printf("ntop  myip: %s\n", myip );
}
