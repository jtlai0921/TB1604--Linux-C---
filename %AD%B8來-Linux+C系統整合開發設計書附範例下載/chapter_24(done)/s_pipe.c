#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

#define MAX 64

int main(void)
{
	int sockfd[2]; /* 用於儲存Unix域套接字描述符的陣列 */
	pid_t pid;
	char buf[MAX];
	int n;

	/* 建立一對未命名的Unix域套接字，其類型為資料流方式的
* sockfd[0]為讀端，sockfd[1]為寫端
 */
	if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1){
		perror("fail to create socket");
		exit(1);
	}

	/* 建立一個子執行緒，父子執行緒使用剛剛建立的未命名的Unix域套接字進行通訊 */
	pid = fork();

	if(pid < 0){ /* 建立子執行緒出錯 */
		perror("fail to fork");
		exit(1);
	}else if(pid == 0){ /* 子執行緒 */
		close(sockfd[0]); /* 關閉讀端，子執行緒向父執行緒輸出內容 */

		strcpy(buf, "hello parent");
		n = strlen(buf);

		if(write(sockfd[1], buf, n) == -1){ /* 向父執行緒發送資訊 */
			perror("fail to write");
exit(1);
		}
		
		printf("the child done\n"); /* 輸出提示資訊，執行緒終止執行 */
	}else{
		close(sockfd[1]); /* 關閉寫端，父執行緒讀取子執行緒的輸出 */
		
		if(read(sockfd[0], buf, MAX) == -1){ /* 讀取子執行緒發送給父執行緒的資訊 */
			perror("fail to read");
			exit(1);
		}

		n = strlen(buf);
		buf[n] = '\0'; /* 增加結束符，便於輸出該資訊 */

		printf("recive from child : %s\n", buf); /* 輸出子執行緒發送給父執行緒的資訊 */
		
		if(wait(NULL) == -1){ /* 回收子執行緒的結束狀態 */
			perror("fail to wait");
			exit(1);
		}

		printf("the parent done\n"); /* 輸出父執行緒結束執行的提示資訊 */
	}

	return 0;
}
