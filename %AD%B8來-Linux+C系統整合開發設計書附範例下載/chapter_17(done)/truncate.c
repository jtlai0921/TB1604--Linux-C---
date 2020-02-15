#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 32

int main(int argc, char *argv[ ])
{
	int fd;
	int len;
	int rest;
	int i;
	char buf[MAX];
	
	if(argc != 3){ /* 根據指令行參數設定擴充後的檔案字節數和需要填充的字節數 */
		len = MAX;
		rest = 0;
	}else{
		len = atoi(argv[1]);
		rest = atoi(argv[2]);
	}

	if(truncate("test.txt", MAX) == -1){ /* 截短操作，將檔案拓展為指定字節數 */
		perror("fail to truncate");
		exit(1);
	}

	/* 增加寫方式開啟檔案，每次寫的內容會自動增加到檔案的結尾 */
	fd = open("test.txt", O_RDWR | O_APPEND);
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}
	
	i = 0;
	while(i < rest){ /* 設定填充內容，餘下的檔案內容填充為字符『0』 */
		buf[i] = '0';
		i++;
	}

	if(write(fd, buf, rest) == -1){ /* 填充檔案 */
		perror("fail to write");
		exit(1);
	}

	close(fd); /* 關閉檔案 */

	return 0;
}
