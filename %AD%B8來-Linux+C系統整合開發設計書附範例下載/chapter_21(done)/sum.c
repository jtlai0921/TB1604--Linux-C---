#include <stdio.h>
#include <stdlib.h>

#define MAX 1024

int main(void)
{
	FILE *fp;
	char buf[MAX];
	int n;
	char *p;
	int letter, number, blank;

	fp = fopen("text.txt", "rb"); /* 開啟該檔案 */
	if(fp == NULL){
		perror("fail to open");
		exit(1);
	}

	letter = 0;
	number = 0;
	blank = 0;
	/* 循環讀取檔案的內容，該檔案可能很大，不能一次讀入到緩衝區中 */
	while((n = fread(buf, sizeof(char), MAX - 1, fp)) > 0){
		buf[n] = '\0'; /* 手動設定結束符 */
		p = buf;

		while(*p != '\0'){ /* 處理每次讀入的檔案內容 */
			if(('a' <= *p && *p <= 'z') || ('A' <= *p && *p <= 'Z')) /* 判斷為字母 */
				letter++;
			if(*p == ' ') /* 判斷為空格 */
				blank++;
			if('0' <= *p && *p <= '9') /* 判斷為數字 */
				number++;
			p++;
		}
	}

	if(n == -1){ /* 讀操作出錯 */
		perror("fail to read");
		exit(1);
	}

	/* 輸出結果，字母數、數字數和空格數 */
	printf("the sum of letter is : %d, the sum of number is : %d, 
		the sum of blank is : %d", letter, number, blank);

	return 0;
}
