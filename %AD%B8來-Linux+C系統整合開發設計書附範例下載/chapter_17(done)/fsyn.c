#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	int i;

	fd = open("test.txt", O_RDWR); /* 開啟一個檔案，其開啟方式為寫入 */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}
	
	i = 0;
	while(i < 3){
		sleep(5); /* 休眠五秒鐘 */

		printf("hello\n");
		if(write(fd, "hello\n", 6) == -1){ /* 向檔案輸出一行字串作為提示資訊 */
			perror("fail to write");
			exit(1);
		}
		
		if(fsync(fd) == -1){ /* 輸出資訊後立即進行檔案同步，保證輸出資訊即時寫回到磁碟上 */
			perror("fail to fsync");
			exit(1);
		}

		i++;
	}

	close(fd); /* 關閉檔案 */

	return 0;
}
