#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define MAX 1024

int main(void)
{
	int fd;
	int n;
	char buf[MAX];

	if(symlink("test.txt", "symbol_link") == -1){ /* 建立一個符號鏈接，目的檔案是test.txt */
		perror("fail to create symbol link");
		exit(1);
	}
	
	printf("already create a symbol-link\n"); /* 輸出提示資訊 */
	
	if((fd = open("symbol_link", O_RDWR)) == -1){ /* 透過符號鏈接開啟該目的檔案 */
		perror("fail to open");
		exit(1);
	}

	if((n = read(fd, buf, MAX)) == -1){ /* 讀目的檔案的內容 */
		perror("fail to read");
		exit(1);
	}
	buf[n] = '\0';

	printf("file content is : %s\n", buf); /* 列印目的檔案的內容 */

	strcat(buf, ", admin");

	if(write(fd, buf, strlen(buf)) == -1){ /* 向目的檔案輸出 */
		perror("fail to write");
		exit(1);
	}

	printf("done\n"); /* 輸出提示資訊 */

	close(fd);

	return 0;
}
