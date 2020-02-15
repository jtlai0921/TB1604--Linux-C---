#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 32

int main(int argc, char *argv[ ])
{
	int fd;
	int len;
	int rest;
	int i;
	char buf[MAX];
	
	if(argc != 3){ /* �ھګ��O��ѼƳ]�w�X�R�᪺�ɮצr�`�ƩM�ݭn��R���r�`�� */
		len = MAX;
		rest = 0;
	}else{
		len = atoi(argv[1]);
		rest = atoi(argv[2]);
	}

	if(truncate("test.txt", MAX) == -1){ /* �I�u�ާ@�A�N�ɮשݮi�����w�r�`�� */
		perror("fail to truncate");
		exit(1);
	}

	/* �W�[�g�覡�}���ɮסA�C���g�����e�|�۰ʼW�[���ɮת����� */
	fd = open("test.txt", O_RDWR | O_APPEND);
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}
	
	i = 0;
	while(i < rest){ /* �]�w��R���e�A�l�U���ɮפ��e��R���r�šy0�z */
		buf[i] = '0';
		i++;
	}

	if(write(fd, buf, rest) == -1){ /* ��R�ɮ� */
		perror("fail to write");
		exit(1);
	}

	close(fd); /* �����ɮ� */

	return 0;
}
