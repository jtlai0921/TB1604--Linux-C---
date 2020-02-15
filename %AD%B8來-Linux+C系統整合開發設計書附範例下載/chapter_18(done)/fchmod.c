#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* 讀操作掩碼，將所有者使用者、組使用者和其它使用者的讀權限全部增加 */
#define READ_MASK S_IRUSR | S_IRGRP | S_IROTH
/* 寫操作掩碼，將所有者使用者、組使用者和其它使用者的寫權限全部增加 */
#define WRITE_MASK S_IWUSR | S_IWGRP | S_IWOTH

int main(void)
{
	int fd;
	struct stat statbuf;

	/* 開啟一個檔案，如果該檔案不存在，則使用讀操作掩碼作為權限字建立該檔案 */
	if((fd = open("test.txt", O_RDONLY | O_CREAT, READ_MASK)) == -1){
		perror("fail to open");
		exit(1);
	}
	
	printf("before changing model\n"); /* 輸出提示資訊 */
	 
	if(fstat(fd, &statbuf) == -1){ /* 在開啟的檔案上得到檔案狀態 */
		perror("fail to get status");
		exit(1);	
	}
	
	if(statbuf.st_mode & S_IRUSR) /* 所有者使用者具有讀檔案的權限 */
		printf("user can read\n");
	if(statbuf.st_mode & S_IRGRP) /* 組使用者具有讀檔案的權限 */
		printf("group user can read\n");
	if(statbuf.st_mode & S_IROTH) /* 其它使用者具有讀檔案的權限 */
		printf("other user can read\n");
	printf("\n");

	/* 使用寫操作權限字改變檔案的權限，
* 改變後檔案的的所有讀權限消失，
* 取而代之的是所有的寫權限 
*/
	if(fchmod(fd, WRITE_MASK) == -1){
		perror("fail to change model");
		exit(1);	
	}
	
	printf("after changing model\n");
	
	if(fstat(fd, &statbuf) == -1){ /* 再次去檔案的狀態 */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("check the file by file-descriptor\n");

	/* 直接使用該檔案的描述符取得檔案狀態，檢查檔案權限是否更新 */
	if(statbuf.st_mode & S_IWUSR) /* 所有者使用者具有寫檔案的權限 */
		printf("user can write\n");
	if(statbuf.st_mode & S_IWGRP) /* 組使用者具有寫檔案的權限 */
		printf("group user can write\n");
	if(statbuf.st_mode & S_IWOTH) /* 其它使用者具有寫檔案的權限 */
		printf("other user can write\n");
printf("\n");
	
	/* 再次從磁碟上取得該檔案的檔案狀態，檢查磁碟上的檔案的權限是否也已經更新 */
	if(stat("test.txt", &statbuf) == -1){
		perror("fail to get status"); 
		exit(1);	
	}

	printf("check the file in the disk\n");
	
	/* 磁碟上的檔案權限也已經更新 */
	if(statbuf.st_mode & S_IWUSR) /* 所有者使用者具有寫檔案的權限 */
		printf("user can write\n");
	if(statbuf.st_mode & S_IWGRP) /* 組使用者具有寫檔案的權限 */
		printf("group user can write\n");
	if(statbuf.st_mode & S_IWOTH) /* 其它使用者具有寫檔案的權限 */
		printf("other user can write\n");	
	printf("\n");

	
	sleep(10); /* 休眠10秒鐘 */
	
	printf("done\n"); /* 列印提示資訊 */
	
	close(fd); /* 檔案關閉，所有緩衝區的內容沖洗到磁碟上 */
 
	return 0;
}
