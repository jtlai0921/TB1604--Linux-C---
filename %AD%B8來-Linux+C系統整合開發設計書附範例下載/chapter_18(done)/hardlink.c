#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* 緩衝區的大小 */
#define MAX 1024

int main(void)
{
	int fd;
	struct stat statbuf;
	char buf[MAX];
	int n;
	
	if(stat("test.txt", &statbuf) == -1){ /* 得到目標檔案的狀態 */
		perror("fail to get status");
		exit(1);	
	}
	
	/* 列印檔案的鏈接數 */
	printf("test.txt, the number of links is : %d\n", statbuf.st_nlink);
	
	/* 目前目錄下建立一個叫做test2.txt的檔案，該檔案和test.txt共用一個i結點 */
	if(link("test.txt", "test2.txt") == -1){
		perror("fail to link");
		exit(1);	
	}
	
	/* 再次取得test.txt檔案的狀態，其鏈接數已經更新 */
	if(stat("test.txt", &statbuf) == -1){
		perror("fail to get status");
		exit(1);	
	}
	
	printf("test.txt, the number of links is : %d\n", statbuf.st_nlink);
	
	/* 得到test2.txt檔案的狀態，這個狀態結構實際上是檔案test.txt的 */
	if(stat("test2.txt", &statbuf) == -1){
		perror("fail to get status");
		exit(1);	
	}
	
	printf("test2.txt, the number of links is : %d\n", statbuf.st_nlink);
	printf("\n");
	
	if((fd = open("test.txt", O_RDWR)) == -1){ /* 開啟檔案test.txt */
		perror("fail to open");
		exit(1);
	}
	
	strcpy(buf, "hello world"); /* 複製字串 */
	
	if((n = write(fd, buf, strlen(buf))) == -1){ /* 向檔案中輸出字串"hello world" */
		perror("fail to write");
		exit(1);	
	}

	close(fd); /* 關閉檔案，輸出的字串寫入到磁碟檔案中 */
	
	if((fd = open("test2.txt", O_RDWR)) == -1){ /* 開啟test2.txt檔案 */
		perror("fail to open");
		exit(1);
	}

	if((n = read(fd, buf, n)) == -1){ /* 讀該檔案的內容 */
		perror("fail to read");
		exit(1);	
	}
	buf[n] = '\0'; /* 增加字串結束標誌，便於列印 */

	printf("content of test2.txt is : %s\n", buf); /* 輸出test2.txt檔案中的內容 */

	close(fd);
	
	/* 刪除test2.txt的目錄項，但是其磁碟檔案沒有受到影響，
	* 仍可以透過另一個鏈接test.txt參考該檔案 
	*/
	if(unlink("test2.txt") == -1){
		perror("fail to unlink");
		exit(1);	
	}
	
	if(stat("test.txt", &statbuf) == -1){ /* 得到test.txt檔案的狀態 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("test.txt, the number of links is : %d\n", statbuf.st_nlink); /* 列印該檔案鏈接計數 */
	
	/* 開啟test.txt檔案，這樣做可以避免該檔案被系統刪除 */
	if((fd = open("test.txt", O_RDWR)) == -1){
		perror("fail to open");
		exit(1);
	}
	
	if(unlink("test.txt") == -1){ /* 現在該檔案的參考計數為0了 */
		perror("fail to unlink");
		exit(1);	
	}
	
	if(fstat(fd, &statbuf) == -1){ /* 得到檔案的狀態 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("test.txt, the number of links is : %d\n", statbuf.st_nlink);
	printf("\n");

	/* 由於檔案仍然開啟，所有該檔案仍可以應用 */
	if((n = read(fd, buf, n)) == -1){
		perror("fail to read");
		exit(1);	
	}
	buf[n] = '\0';

	printf("content of test.txt is : %s\n", buf); /* 輸出結果 */
	
	close(fd); /* 關閉檔案 */
	
	return 0;	
}
