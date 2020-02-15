#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include "iolib.h" /* 使用者自己的I/O函數庫 */

#define MAX_LINE 80
int port = 8000; /*  */

/* 處理函數，用於將大寫字符轉換為小寫字符。參數為需要轉換的字串 */
void my_fun(char * p)
{
	if(p == NULL) /* 空串 */
		return;

	for (; *p != '\0'; p++)
		if(*p >= 'A'&& *p <= 'Z') /* 判斷字符並進行轉換，也可以使用庫函數 */
			*p = *p -'A'+ 'a';
}

int main(void)
{
     struct sockaddr_in sin;
     struct sockaddr_in cin;
     int lfd;
     int cfd;
     int sfd;
int rdy;
int client[FD_SETSIZE]; /* 客戶端連接的套接字描述符陣列 */
     int maxi;
     int maxfd; /* 最大連接數 */
     fd_set rset;
     fd_set allset;
     socklen_t addr_len; /* 地址結構的長度 */
     char buf[MAX_LINE];
     char str[INET_ADDRSTRLEN];
     int i;
     int n;
int len;
int opt = 1; /* 套接字選項 */
	char addr_p[18];

     bzero(&sin, sizeof(sin)); /* 填充地址結構 */
     sin.sin_family = AF_INET;
     sin.sin_addr.s_addr = INADDR_ANY;
     sin.sin_port = htons(port);

	/* 建立一個面對連接的套接字 */
     lfd = socket(AF_INET, SOCK_STREAM, 0);
     if (lfd == -1) {
          perror("call to socket");
          exit(1);
     }
     
	/* 設定套接字選項，使用預設選項 */
setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

/* 綁定套接字到地址結構 */
     n = bind(lfd, (struct sockaddr *) &sin, sizeof(sin));
     if (n == -1) {
          perror("call to bind");
          exit(1);
}

/* 開始監聽連接請求 */
     n = listen(lfd, 20);
     if (n == -1) {
          perror("call to listen"), exit(1);
}

     printf("Accepting connections ...\n"); 

     maxfd = lfd; /* 對最大檔案描述符數進行初始化 */
maxi = -1;

     for (i = 0; i < FD_SETSIZE; i++)  /* 初始化客戶端連接描述符集合 */
          client[i] = -1;        

     FD_ZERO(&allset); /* 清空檔案描述符集合 */
     FD_SET(lfd, &allset); /* 將監聽接字設定在集合內 */

	/* 開始服務器程式的無窮迴圈 */
     while(1){
         rset = allset;

		/* 得到目前可以讀的檔案描述符數 */
         rdy = select(maxfd + 1, &rset, NULL, NULL, NULL);

         if (FD_ISSET(lfd, &rset)){
addr_len = sizeof(cin);

/* 建立一個連接描述符 */
              cfd = accept(lfd, (struct sockaddr *) &cin, &addr_len);
			if(cfd == -1){
				perror("fail to accept");
				exit(1);
			}

/* 尋找一個空閒的位置 */
              for (i = 0; i < FD_SETSIZE; i++)
                   if (client[i] < 0){
                        client[i] = cfd; /* 將處理該客戶端的連接套接字設定到該位置 */
                        break;
}
			
			/* 太多的客戶端連接，服務器拒絕連接，跳出循環 */
			if (i == FD_SETSIZE){
                   printf("too many clients");
                   exit(1);
}

FD_SET(cfd, &allset); /* 設定連接集合 */

if (cfd > maxfd) /* 新的連接描述符 */
                   maxfd = cfd; 
              if (i > maxi)
                   maxi = i;
              if (--rdy <= 0) /* 減少一個連接描述符 */
                   continue;
          }

		for (i = 0; i <= maxi; i++){ /* 對每一個連接描述符做處理 */
               if ((sfd = client[i]) < 0)
                    continue;
               
if (FD_ISSET(sfd, &rset)){
	
                   n = my_read(sfd, buf, MAX_LINE); /* 讀取資料 */
                   if (n == 0){
                        printf("the other side has been closed.\n");
                        fflush(stdout); /* 更新到輸出終端 */
                        close(sfd);
					
                        FD_CLR(sfd, &allset); /* 清空連接描述符陣列 */
                        client[i] = -1;
                   }else{
					/* 將客戶端地址轉換為字串 */
                       	inet_ntop(AF_INET, &cin.sin_addr, addr_p, sizeof(addr_p));    
     
					/* 列印客戶端地址和通訊埠號 */
					printf("client IP is %s, port is %d\n", addr_p, ntohs(sin.sin_port));
printf("content is : %s\n", buf); /* 列印客戶端發送過來的字串 */

					my_fun(buf); /* 呼叫大小寫轉換函數 */
					
                       n = my_write(sfd, buf, len + 1);
					if(n == -1) /* 寫函數出錯 */
						exit(1);
}

                    if (--rdy <= 0)
                         break;      /* 如果沒有可以讀的套接字則退出循環 */
               }
          }
}

close(lfd); /* 關閉鏈接套接字 */

return 0;
}
