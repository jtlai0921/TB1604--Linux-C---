#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int main(void)
{
	struct stat statbuf;
	char buf[1024];
	
	printf("before changing\n\n"); /* ��X���ܸ�T */

	if(stat("test.txt", &statbuf) == -1){ /* �o��ت��ɮת����A��T */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);

	if(lstat("sl", &statbuf) == -1){ /* �ϥ�lstat��Ʊo��Ÿ��챵�����A��T */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("the owner of sl is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of sl is : %d\n", (unsigned int)statbuf.st_gid);
	
	if(chown("sl", 0, -1) == -1){ /* ���ܥت��ɮת��Ҧ��� */
		perror("fail to change owner");
		exit(1);	
	}
	
	printf("after changing\n"); /* ��X���ܸ�T */

	if(stat("test.txt", &statbuf) == -1){ /* �A���o��ت��ɮת��ɮת��A��T */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);

	if(lstat("sl", &statbuf) == -1){ /* �ϥ�lstat��Ʊo��Ÿ��챵�������ɮת��A��T */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("the owner of sl is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of sl is : %d\n", (unsigned int)statbuf.st_gid);
	
	return 0;	
}
