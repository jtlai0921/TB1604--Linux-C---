#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX 100000
#define LEN 1024  /* 使用巨集最為緩衝區的大小 */

int main(int argc, char *argv[ ])
{
	int fd1, fd2; 
	FILE *fp;
	char buf[MAX]; /* 大檔案的緩衝區 */
	int n, rest;
	char *p = buf;
	char content[LEN];

	if(argc != 3){ /* 缺少檔案名 */
		printf("expect args\n");
		exit(1);
	}

	fd1 = open(argv[1], O_RDONLY); /* 輸入檔案 */
	if(fd1 == -1){
		perror("fail to read");
		exit(1);
	}

	fp = fopen(argv[2], "w"); /* 輸出錯誤原因的檔案，使用格式化I/O */
	if(fp == NULL){
		perror("fail to read");
		exit(1);
	}

	fd2 = open("test.txt", O_WRONLY); /* 輸出檔案，低速檔案test.txt檔案 */
	if(fd2 == -1){
		perror("fail to read");
		exit(1);
	}

	rest = read(fd1, buf, MAX); /* 讀檔案的內容到緩衝區 */
	printf("get %d bytes from %s\n", rest, argv[1]);

	while(rest > 0){ /* 當要輸出的內容還有剩餘時繼續輸出 */
		errno = 0;
		n = write(fd2, p, rest); /* 輸出緩衝區內容 */
		
		fprintf(fp, "write %d, errno %s\n", n, strerror(errno)); /* 如果輸出失敗則輸出錯誤原因 */
		if(rest > 0){ /* 計算剩餘的字節數 */
			p += n;
			rest -= n;
		}
	}

	printf("done\n");

	return 0;
}
