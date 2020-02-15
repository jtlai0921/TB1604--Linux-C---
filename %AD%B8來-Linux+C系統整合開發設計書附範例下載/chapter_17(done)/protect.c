#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

/* SIGBUS訊號的處理函數，輸出一行提示資訊 */
void sigbus_handler(int signo)
{
	printf("permission denied\n");
}

int main(void)
{
	int fd;
	char *buf;
	struct stat statbuf;

	if(signal(SIGBUS, sigbus_handler) == SIG_ERR){ /* 設定SIGBUS的訊號處理程式 */
		perror("can』t set handler for SIGALRM");
		exit(0);
	}

	if(stat("test.txt", &statbuf) == -1){ /* 得到一個檔案的狀態資訊，得到檔案的大小 */
		perror("fail to get stat");
		exit(1);
	}

	fd = open("test.txt", O_WRONLY); /* 以寫入方式開啟檔案 */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}

	/* 建立一個記憶體映射區，其存取權限為只讀 */
	buf = (char *)mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if(buf == MAP_FAILED){
		perror("fail to mmap");
		exit(1);
	}
	
	printf("try to write\n");
	strcpy(buf, "China\n"); /* 嘗試寫一個只讀的映射區 */

	/* 將映射區的存取權限改變為讀取寫入 */
	if(mprotect(buf, statbuf.st_size, PROT_READ | PROT_WRITE) == -1){
		perror("fail to alter permission");
		exit(1);
	}

	printf("write again\n");
	strcpy(buf, "China\n"); /* 再次寫一個只讀的映射區 */

	if(munmap(buf, statbuf.st_size) == -1){ /* 取消記憶體映射 */
		perror("fail to munmap");
		exit(1);
	}

	close(fd); /* 關閉檔案 */
	
	return 0;
}
