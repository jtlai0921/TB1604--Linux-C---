#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX 1024
#define PID 6
#define PATH_SIZE 128

int main(void)
{
	FILE *fp;
	pid_t pid;
	char pid_str[PID];
	char path[PATH_SIZE];
	char buf[MAX];

	pid = getpid(); /* 得到執行緒ID */
	sprintf(pid_str, "%d", pid); /* 將執行緒ID由數字轉換為字串 */
	strcpy(path, "/proc/self/task/"); /* 拼接路徑，開啟"/proc/self/task/執行緒ID"目錄下的status檔案 */
	strcat(path, pid_str);
	strcat(path, "/status");

	fp = fopen(path, "r"); /* 開啟該檔案，以只讀的方式開啟 */
	if(fp == NULL){
		perror("fail to open");
		exit(1);
	}

	while(fgets(buf, MAX, fp) != NULL) /* 順序讀取每一行，並且列印 */
		printf("%s", buf);

	fclose(fp); /* 關閉檔案 */

	return 0;
}
