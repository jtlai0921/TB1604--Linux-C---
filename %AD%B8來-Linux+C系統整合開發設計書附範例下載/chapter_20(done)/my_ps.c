#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <unistd.h>  
#include <dirent.h>  
#include <fcntl.h>

#define MAX 1024
#define PATH_SIZE 128

int main(void)
{  
	DIR *dir;
    	struct dirent	*entry; 
    	FILE *fp;
	char path[PATH_SIZE];
	char buf[MAX];

	printf("NAME\tPID\n"); /* 輸出表頭 */

    	if((dir = opendir( "/proc" ) ) == NULL ) { /* 開啟/proc目錄 */
        	perror("fail to open dir");
			return -1; 
    	}  

	while((entry = readdir( dir ) ) != NULL){  
        	if(entry->d_name[0] == '.') /* 跳過目前目錄，proc目錄沒有父目錄 */ 
			continue;
		/* 跳過系統資訊目錄，所有執行緒的目錄全都是數字，而系統資訊目錄全都不是數字 */
		if( '0' <= entry->d_name[0] && entry->d_name[0] <= '9')
			continue;
			
		/* 使用sprintf完成並接路徑，其中兩個%s會由entry->d_name表示的執行緒ID替代 */
		sprintf(path, "/proc/%s/task/%s/status", entry->d_name, entry->d_name);
		
		fp = fopen(path, "r"); /* 開啟檔案 */
		if(fp == NULL){
			perror("fail to open");
			exit(1);
		}

		while(fgets(buf, MAX, fp) != NULL){ /* 讀取每一行 */
			if(buf[0] == 'N'
 			&& buf[1] == 'a'
			&& buf[2] == 'm'
			&& buf[3] == 'e')
				printf("%s\t", &buf[5]); /* 跳過'\t'，輸出狀態資訊 */
			
			if(buf[0] == 'P'
 			&& buf[1] == 'i'
			&& buf[2] == 'd'){
				printf("%s\n", &buf[4]); /* 輸出PID後就結束循環 */
				break;
			}
		}

		fclose(fp); /* 關閉stattus檔案 */
	}
    	
	closedir( dir ); /* 關閉目錄 */
    
	return 0;  
}
