#include <stdio.h>
#include <stdlib.h>

/* 程式使用方法：copy src des
*  本程式並未做輸入的出錯處理，例如檔案路徑的正確性以及目標檔案已經是否存在等
*/
int main(int argc, char *argv[ ])
{
	FILE *fp1, *fp2; /* 源檔案和目標檔案 */
	char buf[1024];
	int n;

	if(argc != 3){ /* 檢查參數個數 */
		printf("wrong command\n");
		exit(1);
	}
	
	if((fp1 = fopen(argv[1], "rb")) == NULL){ /* 開啟源檔案 */
		perror("fail to open");
		exit(1);
	}
	if((fp2 = fopen(argv[2], "wb")) == NULL){ /* 開啟目標檔案 */
		perror("fail to open");
		exit(1);
	}
	
/* 開始複製檔案，檔案可能很大，緩衝一次裝不下，所以使用一個循環進行讀寫 */
	while((n = fread(buf, sizeof(char), 1024, fp1)) > 0){ /* 讀源檔案，直到將檔案內容全部讀完 */
		if(fwrite(buf, sizeof(char), n, fp2) == -1){ /* 將讀出的內容全部寫到目標檔案中去 */
			perror("fail to write");
			exit(1);
		}
	}
	if(n == -1){ /* 如果應為讀入字節小於0而跳出循環則說明出錯了 */
		perror("fail to read");
		exit(1);
	}

	fclose(fp1); /* 關閉源檔案和目標檔案 */
	fclose(fp2);

	return 0;
}
