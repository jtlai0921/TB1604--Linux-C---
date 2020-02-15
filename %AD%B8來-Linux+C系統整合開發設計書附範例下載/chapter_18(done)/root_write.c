#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 1024

int main(int argc, char *argv[ ])
{
	int sfd, dfd;
	struct stat statbuf;
	char buf[MAX];
	int n;

	if(stat(argv[0], &statbuf) == -1){ /* 取得執行程式檔案的狀態資訊 */
		perror("fail to stat");
		exit(1);
	}
	
	if(statbuf.st_mode & S_ISUID != 0) /* 測試組使用者是否設定了"設定使用者ID"位 */
		printf("set user id\n");
else{
		printf("can』t write to root.txt\n");
		exit(1);
	}

if((sfd = open("test.txt", O_RDONLY)) == -1){ /* 開啟源檔案，裡面由需要寫到root.txt中的內容 */
		perror("fail to open");
		exit(1);
	}

if((dfd = open("root.txt", O_WRONLY)) == -1){ /* 開啟root.txt檔案 */
		perror("fail to open");
		exit(1);
	}
	
	while((n = read(sfd, buf, MAX)) >0){ /* 將sfd檔案中的內容寫到dfd檔案中去 */
		if(write(dfd, buf, n) == -1){
			perror("fail to write");
			exit(1);
		}
	}

	if(n < 0){ /* 如果因為read函數傳回-1而跳出循環，則說明read函數出錯 */
		perror("fail to read");
		exit(1);
	}

	printf("done\n"); /* 輸出提示資訊 */

	return 0;
}
