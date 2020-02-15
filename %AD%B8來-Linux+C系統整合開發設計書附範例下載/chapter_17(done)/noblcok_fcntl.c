#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX 100000
#define LEN 1024

int main(int argc, char *argv[ ])
{
	int fd1, fd2; 
	FILE *fp;
	char buf[MAX]; /* 大檔案的緩衝區 */
	int n, rest;
	char *p = buf;
	char content[LEN];
	int flags;

	if(argc != 3){ /* 缺少檔案名 */
		printf("expect args\n");
		exit(1);
	}

	fd1 = open(argv[1], O_RDONLY); /* 開啟輸入檔案 */
	if(fd1 == -1){
		perror("fail to read");
		exit(1);
	}

	fd2 = open(argv[2], O_WRONLY); /* 開啟輸出出錯資訊的檔案 */
	if(fd2 == -1){
		perror("fail to read");
		exit(1);
	}

	fp = fdopen(fd2, "w"); /* 開啟檔案，以只寫的方式 */
	if(fp == NULL){
		perror("fail to open");
		exit(1);
	}

	flags = fcntl(STDOUT_FILENO, F_GETFL, 0); /* 將標準輸出設定為非阻塞形式 */
	if(flags == -1){
		perror("fail to fcntl");
		exit(1);
	}
	flags |= O_NONBLOCK; /* 設定非阻塞標誌 */
	if(fcntl(STDOUT_FILENO, F_SETFL, flags) == -1){ /* 重新設定檔案的狀態標誌 */
		perror("fail to fcntl");
		exit(1);
	}

	rest = read(fd1, buf, MAX); /* 讀入檔案 */
	printf("get %d bytes from %s\n", rest, argv[1]);

	while(rest > 0){ /* 當要輸出的內容還有剩餘時繼續輸出 */
		errno = 0;
		n = write(STDOUT_FILENO, p, rest); /* 輸出緩衝區內容 */
		
		fprintf(fp, "write %d, errno %s\n", n, strerror(errno)); /* 如果輸出失敗則輸出錯誤原因 */
		if(rest > 0){ /* 計算剩餘的字節數 */
			p += n;
			rest -= n;
		}
	}

	printf("done\n");

	close(fd1); /* 關閉檔案 */

	fclose(fp);

	return 0;
}
