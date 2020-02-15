#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd;
	char *p = "hello world\n"; /* ���եΪ��r�� */

	if(chdir("/home/admin/tmp") == -1){ /* ���ܰ�������u�@�ؿ� */
		perror("fail to change dir");
		exit(1);
	}
	
	printf("change dir successfully\n"); /* ��X���ܸ�T */

	if((fd = open("test.txt", O_CREAT | O_RDWR)) == -1){ /* �إߤ@���ɮסA���ɮ׫إߦb�ק�᪺�ؿ� */
		perror("fail to open");
		exit(1);
	}

	if(write(fd, p, strlen(p)) == -1){ /* �V���ɮפ���X���e */
		perror("fail to write");
		exit(1);
	}

	close(fd); /* �����ɮ� */

	return 0;
}
