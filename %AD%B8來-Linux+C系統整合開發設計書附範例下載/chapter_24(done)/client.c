#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include "iolib.h" /* 增加使用者自已的I/O函數庫 */

#define MAX_LINE 80

int main(int argc, char *argv[])
{
    	struct sockaddr_in sin;
    	int port = 8000; /* 通訊埠號，使用8000通訊埠 */
	int s_fd;
	char buf[MAX_LINE];
    	char *str = "test";
    	char addr_p[INET_ADDRSTRLEN];
    	int n;
    
	if(argc == 2)
		str = argv[1];

	memset(&sin, sizeof(sin), 0); /* 設定地址結構 */
    	sin.sin_family = AF_INET;
    	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
    	sin.sin_port = htons(port);
    
    	s_fd = socket(AF_INET, SOCK_DGRAM, 0); /* 建立一個使用UDP協定的套接字 */
    	if(s_fd == -1){
		perror("fail to create socket");
         	exit(1);
     }
	
	/* 使用connect函數與服務器進行連接，連接之後就相當於使用一個TCP的套接字進行通訊了 */
     n = connect(s_fd, (struct sockaddr *) &sin, sizeof(sin));
     if(n == -1){
          perror("fail to connect");
          exit(1);
     }else
		printf("connection has been established\n");

    	n = my_write(s_fd, str, strlen(str) + 1); /* 發送字串，該串包含『\0』結束符 */
	if(n == -1) /* 寫操作失敗，程式退出 */
		exit(1);
	
	/* 讀取服務器程式發回的串，由於是同一台主機通訊，不存在延時的問題。
	* 但是在真正的網絡環境中，要處理讀操作的延時問題。
	*/
	n = my_read(s_fd, buf, MAX_LINE);
	if(n == -1) /* 讀失敗，退出程式 */
		exit(1);	
     
	printf("recive from server: %s\n", buf); /* 列印該串 */
	
    	if(close(s_fd) == -1){ /* 關閉套接字，結束通訊 */
     	perror("fail to close");
		exit(1);
	}

	return 0;
}
