#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 64

/* linux cp指令的簡單實現，指令格式：cp des src
* 成功傳回0，失敗傳回-1，失敗原因儲存在errno中
* argv[1] : 目標檔案名des（本例使用絕對路徑）
* argv[2]: 源檔案名src（本例使用絕對路徑）
*/
int main(int argc, char* argv[])
{
	char buf[MAX];
	int in, out; /* 輸入檔案和輸出檔案 */
	int n;

	if(argc != 3)
		exit(1);

	if((in = open(argv[2], O_RDONLY)) == -1) {/* 源檔案，"只讀"開啟 */
		perror("fail to open");
		exit(1);
	}
	
	/* 目標檔案，該檔案不存在則建立，該檔案存在則覆蓋且只寫開啟 */
	if((out = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT)) == -1){ 
		perror("fail to open");
		exit(1);
	}
	
	while((n = read(in, buf, MAX)) > 0) /* 讀入檔案 */
		if(write(out, buf, n) != n){ /* 實際寫出字節數不等於n，寫出錯 */
			perror("fail to write");
			exit(1);
		}
	
	if(n < 0){ /* 讀入出錯 */
		perror("fail to read");
		exit(1);
	}

	printf("copy done\n"); /* 輸出提示資訊 */

	close(in); /* 關閉兩個檔案 */
	close(out);

	return 0;
}
