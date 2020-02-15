#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int fd;
	int flag;
	char *p = "1st linux"; /* 測試使用的字串 */
	char *q = "2nd linux";

	fd = open("test.txt", O_WRONLY); /* 該檔案只寫開啟 */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}

	if(write(fd, p, strlen(p)) == -1){ /* 輸入新內容，該內容會覆蓋部分舊的內容 */
		perror("fail to write");
		exit(1);
	}

	flag = fcntl(fd, F_GETFL, 0); /* 先使用F_GETFL指令得到檔案狀態標誌 */
	if(flag == -1){
		perror("fail to fcntl");
		exit(1);
	}
	flag |= O_APPEND; /* 將檔案狀態標誌增加"追加寫"選項 */
	if(fcntl(fd, F_SETFL, flag) == -1){ /* 將檔案狀態修改為追加寫 */
		perror("fail to fcntl");
		exit(1);
	}

	if(write(fd, q, strlen(q)) == -1){ /* 再次輸入新內容，該內容會追加在舊內容的後面 */
		perror("fail to write");
		exit(1);
	}

	close(fd); /* 關閉檔案 */

	return 0;
}
