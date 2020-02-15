#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#define MAX 1024

void my_ls(char *root)
{
	DIR * dir;
	struct dirent * ptr;
	int count;
	
	dir = opendir(root); /* 開啟目錄 */
	if(dir == NULL){
		perror("fail to open dir");
		exit(1);
	}

	errno = 0;
	while((ptr = readdir(dir)) != NULL){ /* 順序讀取每一個目錄項 */
		/* 跳過".."和"."兩個目錄 */
		if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0)
      		continue;
      	printf("%s\t", ptr->d_name); /* 列印每一個檔案的檔案名 */
		
		count++; /* 換行計數 */
		if(count %5 == 0)
			printf("\n");
	}
	
	if(errno != 0){ /* 如果errno是0，則說明目錄中目錄項已經全部讀完 */
		perror("fail to read dir");
		exit(1);
	}
	
	closedir(dir); /* 注意一定要關閉目錄 */
}

int main(int argc, char * argv[])
{
	if(argc != 2){
		printf("wrong usage\n");
		exit(1);
	}

	my_ls(argv[1]);

	return 0;
}
