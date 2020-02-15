#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX 1024

int main(void)
{
	int fd;
	off_t off;
	char buf[MAX];

	fd = open("test.txt", O_RDWR); /* 開啟一個檔案 */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}

	printf("before reading\n"); /* 輸出提示資訊 */

	off = lseek(fd, 0, SEEK_CUR); /* 呼叫lseek函數得到目前檔案的讀寫位置 */
	if(off == -1){
		perror("fail to lseek");
		exit(1);
	}
	
	printf("the offset is : %d\n", off); /* 輸出提示資訊 */

	if(read(fd, buf, 5) == -1){ /* 讀取5個字節的檔案內容 */
		perror("fail ot read");
		exit(1);
	}

	printf("after reading\n"); /* 輸出提示資訊 */
	
	off = lseek(fd, 0, SEEK_CUR); /* 再次呼叫lseek函數得到目前檔案的讀寫位置 */
	if(off == -1){
		perror("fail to lseek");
		exit(errno);
	}
	
	printf("the offset is : %d\n", off); /* 輸出結果 */

	close(fd); /* 關閉檔案 */

	return 0;
}
