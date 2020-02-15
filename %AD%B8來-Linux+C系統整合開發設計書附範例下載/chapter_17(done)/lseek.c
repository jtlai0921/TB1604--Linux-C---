#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX 1024

int main(void)
{
	int fd;
	off_t off;
	char buf[MAX];

	fd = open("test.txt", O_RDWR); /* �}�Ҥ@���ɮ� */
	if(fd == -1){
		perror("fail to open");
		exit(1);
	}

	printf("before reading\n"); /* ��X���ܸ�T */

	off = lseek(fd, 0, SEEK_CUR); /* �I�slseek��Ʊo��ثe�ɮת�Ū�g��m */
	if(off == -1){
		perror("fail to lseek");
		exit(1);
	}
	
	printf("the offset is : %d\n", off); /* ��X���ܸ�T */

	if(read(fd, buf, 5) == -1){ /* Ū��5�Ӧr�`���ɮפ��e */
		perror("fail ot read");
		exit(1);
	}

	printf("after reading\n"); /* ��X���ܸ�T */
	
	off = lseek(fd, 0, SEEK_CUR); /* �A���I�slseek��Ʊo��ثe�ɮת�Ū�g��m */
	if(off == -1){
		perror("fail to lseek");
		exit(errno);
	}
	
	printf("the offset is : %d\n", off); /* ��X���G */

	close(fd); /* �����ɮ� */

	return 0;
}
