#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

/* SIGBUS�T�����B�z��ơA��X�@�洣�ܸ�T */
void sigbus_handler(int signo)
{
	printf("permission denied\n");
}

int main(void)
{
	int fd;
	char *buf;
	struct stat statbuf;

	if(signal(SIGBUS, sigbus_handler) == SIG_ERR){ /* �]�wSIGBUS���T���B�z�{�� */
		perror("can�zt set handler for SIGALRM");
		exit(0);
	}

	if(stat("test.txt", &statbuf) == -1){ /* �o��@���ɮת����A��T�A�o���ɮת��j�p */
		perror("fail to get stat");
		exit(1);
	}

	fd = open("test.txt", O_WRONLY); /* �H�g�J�覡�}���ɮ� */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}

	/* �إߤ@�ӰO����M�g�ϡA��s���v�����uŪ */
	buf = (char *)mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if(buf == MAP_FAILED){
		perror("fail to mmap");
		exit(1);
	}
	
	printf("try to write\n");
	strcpy(buf, "China\n"); /* ���ռg�@�ӥuŪ���M�g�� */

	/* �N�M�g�Ϫ��s���v�����ܬ�Ū���g�J */
	if(mprotect(buf, statbuf.st_size, PROT_READ | PROT_WRITE) == -1){
		perror("fail to alter permission");
		exit(1);
	}

	printf("write again\n");
	strcpy(buf, "China\n"); /* �A���g�@�ӥuŪ���M�g�� */

	if(munmap(buf, statbuf.st_size) == -1){ /* �����O����M�g */
		perror("fail to munmap");
		exit(1);
	}

	close(fd); /* �����ɮ� */
	
	return 0;
}
