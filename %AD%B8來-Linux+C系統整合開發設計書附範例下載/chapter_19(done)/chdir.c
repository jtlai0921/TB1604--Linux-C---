#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd;
	char *p = "hello world\n"; /* 測試用的字串 */

	if(chdir("/home/admin/tmp") == -1){ /* 改變執行緒的工作目錄 */
		perror("fail to change dir");
		exit(1);
	}
	
	printf("change dir successfully\n"); /* 輸出提示資訊 */

	if((fd = open("test.txt", O_CREAT | O_RDWR)) == -1){ /* 建立一個檔案，該檔案建立在修改後的目錄 */
		perror("fail to open");
		exit(1);
	}

	if(write(fd, p, strlen(p)) == -1){ /* 向該檔案中輸出內容 */
		perror("fail to write");
		exit(1);
	}

	close(fd); /* 關閉檔案 */

	return 0;
}
