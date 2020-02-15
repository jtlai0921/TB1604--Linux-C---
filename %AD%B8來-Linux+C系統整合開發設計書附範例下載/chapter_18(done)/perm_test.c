#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	int fd;
	struct stat statbuf;
	pid_t pid;
	uid_t ruid, euid; /* ����������ID�M����ID */
	
	if(stat("test.txt", &statbuf) == -1){ /* ���o�ɮת����A */
		perror("fail to stat");
		exit(1);
	}

	/* �o����������ڨϥΪ�ID�M���ĨϥΪ�ID */
	euid = getuid(); 
	euid = geteuid();

	/* �C�L���������ڨϥΪ�ID�M���ĨϥΪ�ID */
	printf("real id is : %u, effective id is : %u \n", (unsigned int)ruid, (unsigned int)euid);
	/* �C�L�ɮשҦ���ID */
	printf("file owner is : %u\n", statbuf.st_uid);

	if(access("test.txt", R_OK) == -1){ /* �����ɮת��v�� */
		perror("fail to access");
		exit(1);
	}
	
	printf("access successfully\n"); /* ��X���ܸ�T */

	if((fd = open("test.txt", O_RDONLY)) == -1){ /* �p�GŪ�v�����զ��\�A���ն}���ɮ� */
		perror("fail to open");
		exit(1);
	}
	
	printf("ready to read\n"); /* ��X���ܸ�T */

	close(fd); /* �����ɮ� */
	
	return 0;
}
