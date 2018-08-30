/*
* 解析arp包
*
*/

#include <stdio.h>
#define MAC_LEN 18
#define PACKET_LEN 48
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
int main() 
{
	int sockfd;
	sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sockfd == -1) {
		perror("socket");
		return -1;

	}
	//接收arp包
	unsigned char buf[PACKET_LEN + 1] = {0};
	unsigned char src_mac[MAC_LEN] = {0};
	unsigned char dest_mac[MAC_LEN] = {0};
	ssize_t len;
	while(1) {
		memset(buf, 0, PACKET_LEN);
		memset(src_mac, 0, MAC_LEN);
		memset(dest_mac, 0 , MAC_LEN);
		//读取数据
		len = recvfrom(sockfd,buf, PACKET_LEN, 0,
                        NULL, NULL);
		//解析数据
		sprintf(dest_mac, "%2x:%2x:%2x:%2x:%2x:%2x", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

		sprintf(src_mac, "%2x:%2x:%2x:%2x:%2x:%2x", buf[6], buf[7], buf[8], buf[9], buf[10], buf[11]);
		printf("%s------%s", dest_mac, src_mac);

	}
	close(sockfd);
	return 0;
}
