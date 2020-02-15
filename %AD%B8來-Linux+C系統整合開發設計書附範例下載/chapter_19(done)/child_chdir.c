#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 1024

int main(void)
{
	pid_t pid;
	char buf[MAX];

	if(getcwd(buf, MAX) == NULL){ /* �o��ק�᪺�u�@�ؿ� */
		perror("fail to get pwd");
		exit(1);
	}

	printf("the parent�zs pwd is : %s\n", buf);

	pid = fork(); /* �إߤ@�Ӥl����� */

	if(pid == -1){
		perror("fail to fork");
		exit(1);
	}else if(pid == 0){
		printf("the child\n");

		if(chdir("/home/usr/") == -1){ /* ���ܰ�������u�@�ؿ� */
			perror("fail to change dir");
			exit(1);
		}

		if(getcwd(buf, MAX) == NULL){ /* �o��l��������u�@�ؿ� */
			perror("fail to get pwd");
			exit(1);
		}

		printf("the child�zs pwd is : %s\n", buf); /* �C�L�l������ק�᪺�u�@�ؿ� */
	}else{
		if(wait(NULL) == -1){ /* �קK�L�Ͱ���� */
			perror("fail to wait");
			exit(1);
		}

		if(getcwd(buf, MAX) == NULL){ /* �A���o�����������u�@�ؿ� */
			perror("fail to get pwd");
			exit(1);
		}

		printf("the parent�zs pwd is : %s\n", buf); /* ��X���ܸ�T */
	}

	return 0;
}
