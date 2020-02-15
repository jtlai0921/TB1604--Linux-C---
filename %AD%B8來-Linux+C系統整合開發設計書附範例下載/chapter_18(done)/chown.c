#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	struct stat statbuf;
	
	if(stat("test.txt", &statbuf) == -1){ /* 為改變所有者前檔案的狀態 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("before changing owner\n"); /* 列印檔案的所有者使用者ID和組ID */
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);
	printf("\n");
	
	if(chown("test.txt", 0, -1) == -1){ /* 將檔案的所有者修改為根使用者，不改變組使用者ID */
		perror("fail to change owner");
		exit(1);	
	}
	
	if(stat("test.txt", &statbuf) == -1){ /* 再次取得檔案的狀態資訊 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("after changing owner\n");
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid); /* 輸出結果 */
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);
	printf("\n");
	
	if(chown("test.txt", 100, -1) == -1){ /* 將檔案的所有者修改改為一個根本不存在的使用者 */
		perror("fail to change owner");
		exit(1);	
	}
	
	if(stat("test.txt", &statbuf) == -1){ /* 得到檔案狀態資訊 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("nonexsit owner\n");
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid); /* 輸出結果 */
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);
	
	return 0;	
}
