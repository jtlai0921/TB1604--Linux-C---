#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 1024

int main(void)
{
	pid_t pid;
	char buf[MAX];

	if(getcwd(buf, MAX) == NULL){ /* 得到修改後的工作目錄 */
		perror("fail to get pwd");
		exit(1);
	}

	printf("the parent』s pwd is : %s\n", buf);

	pid = fork(); /* 建立一個子執行緒 */

	if(pid == -1){
		perror("fail to fork");
		exit(1);
	}else if(pid == 0){
		printf("the child\n");

		if(chdir("/home/usr/") == -1){ /* 改變執行緒的工作目錄 */
			perror("fail to change dir");
			exit(1);
		}

		if(getcwd(buf, MAX) == NULL){ /* 得到子執行緒的工作目錄 */
			perror("fail to get pwd");
			exit(1);
		}

		printf("the child』s pwd is : %s\n", buf); /* 列印子執行緒修改後的工作目錄 */
	}else{
		if(wait(NULL) == -1){ /* 避免殭屍執行緒 */
			perror("fail to wait");
			exit(1);
		}

		if(getcwd(buf, MAX) == NULL){ /* 再次得到父執行緒的工作目錄 */
			perror("fail to get pwd");
			exit(1);
		}

		printf("the parent』s pwd is : %s\n", buf); /* 輸出提示資訊 */
	}

	return 0;
}
