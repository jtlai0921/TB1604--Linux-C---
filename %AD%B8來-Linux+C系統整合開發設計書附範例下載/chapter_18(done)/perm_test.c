#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	int fd;
	struct stat statbuf;
	pid_t pid;
	uid_t ruid, euid; /* 執行緒的實際ID和有效ID */
	
	if(stat("test.txt", &statbuf) == -1){ /* 取得檔案的狀態 */
		perror("fail to stat");
		exit(1);
	}

	/* 得到執行緒的實際使用者ID和有效使用者ID */
	euid = getuid(); 
	euid = geteuid();

	/* 列印執行緒的實際使用者ID和有效使用者ID */
	printf("real id is : %u, effective id is : %u \n", (unsigned int)ruid, (unsigned int)euid);
	/* 列印檔案所有者ID */
	printf("file owner is : %u\n", statbuf.st_uid);

	if(access("test.txt", R_OK) == -1){ /* 測試檔案的權限 */
		perror("fail to access");
		exit(1);
	}
	
	printf("access successfully\n"); /* 輸出提示資訊 */

	if((fd = open("test.txt", O_RDONLY)) == -1){ /* 如果讀權限測試成功，嘗試開啟檔案 */
		perror("fail to open");
		exit(1);
	}
	
	printf("ready to read\n"); /* 輸出提示資訊 */

	close(fd); /* 關閉檔案 */
	
	return 0;
}
