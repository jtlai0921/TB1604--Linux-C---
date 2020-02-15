#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	int i;

	fd = open("test.txt", O_RDWR); /* �}�Ҥ@���ɮסA��}�Ҥ覡���g�J */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}
	
	i = 0;
	while(i < 3){
		sleep(5); /* ��v������ */

		printf("hello\n");
		if(write(fd, "hello\n", 6) == -1){ /* �V�ɮ׿�X�@��r��@�����ܸ�T */
			perror("fail to write");
			exit(1);
		}
		
		if(fsync(fd) == -1){ /* ��X��T��ߧY�i���ɮצP�B�A�O�ҿ�X��T�Y�ɼg�^��ϺФW */
			perror("fail to fsync");
			exit(1);
		}

		i++;
	}

	close(fd); /* �����ɮ� */

	return 0;
}
