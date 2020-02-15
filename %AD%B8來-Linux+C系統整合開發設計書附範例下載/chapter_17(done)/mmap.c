#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	int fd;
	char *buf;
	int i;
	struct stat statbuf;

	if(stat("test.txt", &statbuf) == -1){ /* 得到一個檔案的狀態資訊，得到檔案的大小 */
		perror("fail to get stat");
		exit(1);
	}

	fd = open("test.txt", O_RDONLY); /* 以只讀方式開啟檔案 */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}

	/* 建立一個記憶體映射，起始地址由系統為使用者選擇，並作為傳回值傳回
	* 建立的映射區的大小為開啟的檔案的大小
	* 存取權限為只讀，屬性為不會寫到磁碟，防止對其進行寫的誤操作
	*/
	buf = (char *)mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(buf == MAP_FAILED){
		perror("fail to mmap");
		exit(1);
	}

	i = 0;
	while(i < statbuf.st_size){ /* 輸出每一個字符，注意mmap函數不會增加』\0』結束符 */
		printf("%c", buf[i]);
		i++;
	}
	printf("\n");
	
	if(munmap(buf, statbuf.st_size) == -1){ /* 取消記憶體映射 */
		perror("fail to munmap");
		exit(1);
	}

	close(fd); /* 關閉檔案 */
	
	return 0;
}
