/*
*	获取指定ip地址的机器的mac地址
*	1.发送有个arp请求包
*	2.接收对方的arp应答包  arp是广播
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
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define MAC_LEN 18
#define PACKET_LEN 48
int main() 
{
	int sockfd;
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sockfd == -1) {
        perror("socket");
        return -1;

    }

	//组装arp请求包
	unsigned char buf[PACKET_LEN + 1] = {
		//对方的mac
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		//自己的mac地址
		0x00, 0x0c, 0x29, 0x7e, 0x51, 0x27,
		//arp帧类型
		0x08, 0x06,
		//硬件类型
		0x00, 0x01,
		//协议类型
		0x08, 0x00,
		//硬件地址长度
		0x06, 0x04,
		//op
		0x00, 0x01,
		//发送端以太网地址
		0x00, 0x0c, 0x29, 0x7e, 0x51, 0x27,
		//发送端ip地址
		192,168,31,110,
		//对方的以太网地址
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		//对方的ip地址
		192,168,31,28
	};

	//把网卡的名字告诉链路层
	struct ifreq req;
	memset(&req, 0, sizeof(req));
	strcpy(req.ifr_name, "ens33");
	ioctl(sockfd, SIOCGIFINDEX, &req);

	//初始化地址结构体
	struct sockaddr_ll addr;
	socklen_t addr_len = sizeof(addr);
	memset(&addr, 0, addr_len);
	addr.sll_ifindex = req.ifr_ifindex;

	//发送apr请求
	ssize_t len;
	
	len = sendto(sockfd, buf, PACKET_LEN, 0,
                      (const struct sockaddr *)&addr, addr_len);
	
	if(len > 0) {
		printf("发送成功");
	}else {
		printf("发送失败");
	}


	//接收应答包
	 //接收arp包
    unsigned char src_mac[MAC_LEN] = {0};
    unsigned char dest_mac[MAC_LEN] = {0};
    while(1) {
        memset(buf, 0, PACKET_LEN);
        memset(src_mac, 0, MAC_LEN);
        memset(dest_mac, 0 , MAC_LEN);
        //读取数据
        len = recvfrom(sockfd,buf, PACKET_LEN, 0,
                        NULL, NULL);
        //解析数据
		if(buf[21] == 2) {
			sprintf(dest_mac, "%2x:%2x:%2x:%2x:%2x:%2x", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

        	sprintf(src_mac, "%2x:%2x:%2x:%2x:%2x:%2x", buf[6], buf[7], buf[8], buf[9], buf[10], buf[11]);
        	printf("%s------%s", dest_mac, src_mac);
		}

    }
	close(sockfd);
	return 0;
}


