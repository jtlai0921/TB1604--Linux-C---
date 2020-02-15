#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* �w�İϪ��j�p */
#define MAX 1024

int main(void)
{
	int fd;
	struct stat statbuf;
	char buf[MAX];
	int n;
	
	if(stat("test.txt", &statbuf) == -1){ /* �o��ؼ��ɮת����A */
		perror("fail to get status");
		exit(1);	
	}
	
	/* �C�L�ɮת��챵�� */
	printf("test.txt, the number of links is : %d\n", statbuf.st_nlink);
	
	/* �ثe�ؿ��U�إߤ@�ӥs��test2.txt���ɮסA���ɮשMtest.txt�@�Τ@��i���I */
	if(link("test.txt", "test2.txt") == -1){
		perror("fail to link");
		exit(1);	
	}
	
	/* �A�����otest.txt�ɮת����A�A���챵�Ƥw�g��s */
	if(stat("test.txt", &statbuf) == -1){
		perror("fail to get status");
		exit(1);	
	}
	
	printf("test.txt, the number of links is : %d\n", statbuf.st_nlink);
	
	/* �o��test2.txt�ɮת����A�A�o�Ӫ��A���c��ڤW�O�ɮ�test.txt�� */
	if(stat("test2.txt", &statbuf) == -1){
		perror("fail to get status");
		exit(1);	
	}
	
	printf("test2.txt, the number of links is : %d\n", statbuf.st_nlink);
	printf("\n");
	
	if((fd = open("test.txt", O_RDWR)) == -1){ /* �}���ɮ�test.txt */
		perror("fail to open");
		exit(1);
	}
	
	strcpy(buf, "hello world"); /* �ƻs�r�� */
	
	if((n = write(fd, buf, strlen(buf))) == -1){ /* �V�ɮפ���X�r��"hello world" */
		perror("fail to write");
		exit(1);	
	}

	close(fd); /* �����ɮסA��X���r��g�J��Ϻ��ɮפ� */
	
	if((fd = open("test2.txt", O_RDWR)) == -1){ /* �}��test2.txt�ɮ� */
		perror("fail to open");
		exit(1);
	}

	if((n = read(fd, buf, n)) == -1){ /* Ū���ɮת����e */
		perror("fail to read");
		exit(1);	
	}
	buf[n] = '\0'; /* �W�[�r�굲���лx�A�K��C�L */

	printf("content of test2.txt is : %s\n", buf); /* ��Xtest2.txt�ɮפ������e */

	close(fd);
	
	/* �R��test2.txt���ؿ����A���O��Ϻ��ɮרS������v�T�A
	* ���i�H�z�L�t�@���챵test.txt�ѦҸ��ɮ� 
	*/
	if(unlink("test2.txt") == -1){
		perror("fail to unlink");
		exit(1);	
	}
	
	if(stat("test.txt", &statbuf) == -1){ /* �o��test.txt�ɮת����A */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("test.txt, the number of links is : %d\n", statbuf.st_nlink); /* �C�L���ɮ��챵�p�� */
	
	/* �}��test.txt�ɮסA�o�˰��i�H�קK���ɮ׳Q�t�ΧR�� */
	if((fd = open("test.txt", O_RDWR)) == -1){
		perror("fail to open");
		exit(1);
	}
	
	if(unlink("test.txt") == -1){ /* �{�b���ɮת��Ѧҭp�Ƭ�0�F */
		perror("fail to unlink");
		exit(1);	
	}
	
	if(fstat(fd, &statbuf) == -1){ /* �o���ɮת����A */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("test.txt, the number of links is : %d\n", statbuf.st_nlink);
	printf("\n");

	/* �ѩ��ɮפ��M�}�ҡA�Ҧ����ɮפ��i�H���� */
	if((n = read(fd, buf, n)) == -1){
		perror("fail to read");
		exit(1);	
	}
	buf[n] = '\0';

	printf("content of test.txt is : %s\n", buf); /* ��X���G */
	
	close(fd); /* �����ɮ� */
	
	return 0;	
}
