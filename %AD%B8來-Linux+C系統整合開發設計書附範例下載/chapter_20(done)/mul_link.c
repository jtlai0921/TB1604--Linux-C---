#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAX 1024

int main(void)
{
	int fd;
	char buf[MAX];
	int n;

if(symlink("test.txt", "sl") == -1){ /* 建立一個符號鏈接，指向test.txt檔案 */
		perror("fail to create symbol link");
		exit(1);
	}
	
	/* 再次建立一個符號鏈接，指向sl。就此形成了一個符號鏈接鏈：sl2->sl->test.txt
	* test.txt檔案是目的檔案，也是該符號鏈接鏈的重點 
	*/
if(symlink("sl", "sl2") == -1){ 
		perror("fail to create symbol link");
		exit(1);
	}
		
	if((fd = open("sl2", O_RDWR)) == -1){ /* 開啟的實際上是目的檔案test.txt */
		perror("fail to open");
		exit(1);
	}

	printf("already create symbol-links\n");

	if((n = read(fd, buf, MAX)) == -1){ /* 讀檔案的內容 */
		perror("fail to read");
		exit(1);
	}
	buf[n] = '\0';

	printf("file content is : %s\n", buf); /* 輸出結果*/

	strcat(buf, ", admin");

	if(write(fd, buf, strlen(buf)) == -1){ /* 向該檔案寫內容 */
		perror("fail to write");
		exit(1);
	}

	printf("done\n"); /* 輸出提示資訊 */

	close(fd);

	return 0;
}
