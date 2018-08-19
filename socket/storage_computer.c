#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
/*
*  测试数据在计算机中的存储方式
*/
int main(void)
{
    int a = 0x80402010;
    //a = htonl(a);
    char *p = (char *)&a;
	
    printf("p=%#hhx\n", *p);

    if (*p == 0x34) {
        printf("little endian\n");    
    } else if (*p == 0x12) {
        printf("big endia\n");    
    } else {
        printf("unknown endia\n");    
    }
    return 0;
}
