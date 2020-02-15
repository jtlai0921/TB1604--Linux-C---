#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char * argv[])
{
	char *str;
	int i;
	struct stat buf; /* 儲存檔案狀態資訊 */
	
	for(i = 1; i < argc; i++){
		printf("%s ", argv[i]);

		if(stat(argv[i], &buf) == -1){ /* 得到檔案的狀態 */
			perror("fail to stat");
			continue;
		}

		if(S_ISREG(buf.st_mode)) /* 普通檔案 */
			printf("regular file \n");
		else if(S_ISDIR(buf.st_mode)) /* 目錄檔案 */
			printf( "directory file\n");
		else if(S_ISCHR(buf.st_mode)) /* 字符特殊檔案 */
			printf( "character special file\n");
		else if(S_ISBLK(buf.st_mode)) /* 塊特殊檔案 */
			printf( "block special file\n");
		else if(S_ISFIFO(buf.st_mode)) /*命名管線檔案 */
			printf( "FIFO file\n");
		else if(S_ISSOCK(buf.st_mode)) /* 套接字檔案 */
			printf( "socket file\n");
		else if(S_ISLNK(buf.st_mode)) /* 符號鏈接檔案 */
			printf( "symbolic link file\n");
		else /* 未知類型 */
			printf( "unkonw\n");
}
	
return 0;
}
