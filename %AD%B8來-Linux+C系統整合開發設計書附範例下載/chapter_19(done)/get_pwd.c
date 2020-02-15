#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 1024

int main(void)
{
	pid_t pid;
	char buf[MAX];
	
	if(chdir("/home/admin/tmp") == -1){ /* 改變執行緒的工作目錄 */
		perror("fail to change dir");
		exit(1);
	}

	if(getcwd(buf, MAX) == NULL){ /* 得到修改後的工作目錄 */
		perror("fail to get pwd");
		exit(1);
	}

	printf("the parent : %s\n", buf); /* 列印修改後的工作目錄 */

	pid = fork(); /* 建立一個子執行緒 */

	if(pid == -1){
		perror("fail to fork");
		exit(1);
	}else if(pid == 0){
		if(getcwd(buf, MAX) == NULL){ /* 得到子執行緒的工作目錄 */
			perror("fail to get pwd");
			exit(1);
		}

		printf("the child : %s\n", buf);
	}else{
		if(wait(NULL) == -1){ /* 避免殭屍執行緒 */
			perror("fail to wait");
			exit(1);
		}
	}

	return 0;
}
