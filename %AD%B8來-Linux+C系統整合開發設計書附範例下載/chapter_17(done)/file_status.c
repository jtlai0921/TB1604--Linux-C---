#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int fd;
	int flag;
	char *p = "1st linux"; /* ���ըϥΪ��r�� */
	char *q = "2nd linux";

	fd = open("test.txt", O_WRONLY); /* ���ɮץu�g�}�� */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}

	if(write(fd, p, strlen(p)) == -1){ /* ��J�s���e�A�Ӥ��e�|�л\�����ª����e */
		perror("fail to write");
		exit(1);
	}

	flag = fcntl(fd, F_GETFL, 0); /* ���ϥ�F_GETFL���O�o���ɮת��A�лx */
	if(flag == -1){
		perror("fail to fcntl");
		exit(1);
	}
	flag |= O_APPEND; /* �N�ɮת��A�лx�W�["�l�[�g"�ﶵ */
	if(fcntl(fd, F_SETFL, flag) == -1){ /* �N�ɮת��A�קאּ�l�[�g */
		perror("fail to fcntl");
		exit(1);
	}

	if(write(fd, q, strlen(q)) == -1){ /* �A����J�s���e�A�Ӥ��e�|�l�[�b�¤��e���᭱ */
		perror("fail to write");
		exit(1);
	}

	close(fd); /* �����ɮ� */

	return 0;
}
