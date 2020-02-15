#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	int fd;
	char *buf;
	int i;
	struct stat statbuf;

	if(stat("test.txt", &statbuf) == -1){ /* �o��@���ɮת����A��T�A�o���ɮת��j�p */
		perror("fail to get stat");
		exit(1);
	}

	fd = open("test.txt", O_RDONLY); /* �H�uŪ�覡�}���ɮ� */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}

	/* �إߤ@�ӰO����M�g�A�_�l�a�}�Ѩt�ά��ϥΪ̿�ܡA�ç@���Ǧ^�ȶǦ^
	* �إߪ��M�g�Ϫ��j�p���}�Ҫ��ɮת��j�p
	* �s���v�����uŪ�A�ݩʬ����|�g��ϺСA������i��g���~�ާ@
	*/
	buf = (char *)mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(buf == MAP_FAILED){
		perror("fail to mmap");
		exit(1);
	}

	i = 0;
	while(i < statbuf.st_size){ /* ��X�C�@�Ӧr�šA�`�Nmmap��Ƥ��|�W�[�z\0�z������ */
		printf("%c", buf[i]);
		i++;
	}
	printf("\n");
	
	if(munmap(buf, statbuf.st_size) == -1){ /* �����O����M�g */
		perror("fail to munmap");
		exit(1);
	}

	close(fd); /* �����ɮ� */
	
	return 0;
}
