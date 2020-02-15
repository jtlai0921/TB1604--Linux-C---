#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	struct stat statbuf;
	
	if(stat("test.txt", &statbuf) == -1){ /* �����ܩҦ��̫e�ɮת����A */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("before changing owner\n"); /* �C�L�ɮת��Ҧ��̨ϥΪ�ID�M��ID */
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid);
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);
	printf("\n");
	
	if(chown("test.txt", 0, -1) == -1){ /* �N�ɮת��Ҧ��̭קאּ�ڨϥΪ̡A�����ܲըϥΪ�ID */
		perror("fail to change owner");
		exit(1);	
	}
	
	if(stat("test.txt", &statbuf) == -1){ /* �A�����o�ɮת����A��T */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("after changing owner\n");
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid); /* ��X���G */
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);
	printf("\n");
	
	if(chown("test.txt", 100, -1) == -1){ /* �N�ɮת��Ҧ��̭ק�אּ�@�Ӯڥ����s�b���ϥΪ� */
		perror("fail to change owner");
		exit(1);	
	}
	
	if(stat("test.txt", &statbuf) == -1){ /* �o���ɮת��A��T */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("nonexsit owner\n");
	printf("the owner of test.txt is : %d\n", (unsigned int)statbuf.st_uid); /* ��X���G */
	printf("the group of test.txt is : %d\n", (unsigned int)statbuf.st_gid);
	
	return 0;	
}
