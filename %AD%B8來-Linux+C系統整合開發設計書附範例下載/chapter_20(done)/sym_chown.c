#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int main(void)
{
	struct stat statbuf;
	char buf[1024];
	
	printf("before changing\n\n"); /* 輸出提示資訊 */

	if(stat("test.txt", &statbuf) == -1){ /* 得到目的檔案的狀態資訊 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);

	if(lstat("sl", &statbuf) == -1){ /* 使用lstat函數得到符號鏈接的狀態資訊 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("the owner of sl is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of sl is : %d\n", (unsigned int)statbuf.st_gid);
	
	if(chown("sl", 0, -1) == -1){ /* 改變目的檔案的所有者 */
		perror("fail to change owner");
		exit(1);	
	}
	
	printf("after changing\n"); /* 輸出提示資訊 */

	if(stat("test.txt", &statbuf) == -1){ /* 再次得到目的檔案的檔案狀態資訊 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);

	if(lstat("sl", &statbuf) == -1){ /* 使用lstat函數得到符號鏈接本身的檔案狀態資訊 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("the owner of sl is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of sl is : %d\n", (unsigned int)statbuf.st_gid);
	
	return 0;	
}
