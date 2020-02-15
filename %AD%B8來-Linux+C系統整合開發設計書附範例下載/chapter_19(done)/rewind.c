#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

int main(void)
{
	DIR *dir;
	struct dirent * d_ptr;

	dir=opendir("/home/admin"); /* 開啟目錄 */
	if(dir == NULL){
		perror("fail to open dir");
		exit(1);
	}
	
	while((d_ptr = readdir(dir))!=NULL){ /* 順序輸出目錄中的每個檔案的檔案名 */
		printf("dir name is :%s \n",d_ptr->d_name);
	}

	printf("read again\n");
	rewinddir(dir); /* 目錄檔案的讀寫位置歸零 */
	
	while((d_ptr = readdir(dir))!=NULL){ /* 再次輸出每個檔案 */
		printf("dir name is :%s\n",d_ptr->d_name);
	}

	closedir(dir); /* 關閉目錄 */

	return 0;
}
