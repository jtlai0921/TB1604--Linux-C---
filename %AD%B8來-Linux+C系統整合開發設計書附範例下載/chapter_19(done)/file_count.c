#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#define MAX 1024

/* get_file_count函數的參數是一個路徑，表示目錄樹的根。
* 該函數傳回參數指定目錄下所用普通檔案的數目。
*/
int get_file_count(char *root)
{
	DIR * dir;
	struct dirent * ptr;
	int total = 0; /* 檔案總數 */
	char path[MAX];
	
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
      	if(ptr->d_type == DT_DIR){ /* 如果是目錄，則遞歸呼叫get_file_count函數 */
      		sprintf(path, "%s/%s", root, ptr->d_name);
      		total += get_file_count(path);
      	}
      	if(ptr->d_type == DT_REG) /* 如果是普通檔案，則總數遞增 */
      		total++;	
	}

	if(errno != 0){ /* 如果errno是0，則說明目錄中目錄項已經全部讀完 */
		perror("fail to read dir");
		exit(1);
	}
	
	closedir(dir); /* 注意一定要關閉目錄 */

	return total; /* 傳回普通檔案數 */
}

int main(int argc, char * argv[])
{
	int total;
	
	if(argc != 2){ /* 指令行參數錯誤 */
		printf("wrong usage\n");
		exit(1);
	}

	total = get_file_count(argv[1]); /* 得到檔案數目 */

	printf("%s has %d files\n", argv[1], total);

	return 0;
}
