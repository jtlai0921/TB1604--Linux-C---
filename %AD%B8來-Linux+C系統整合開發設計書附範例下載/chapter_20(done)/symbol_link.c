#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define MAX 1024

int main(void)
{
	int fd;
	int n;
	char buf[MAX];

	if(symlink("test.txt", "symbol_link") == -1){ /* �إߤ@�ӲŸ��챵�A�ت��ɮ׬Otest.txt */
		perror("fail to create symbol link");
		exit(1);
	}
	
	printf("already create a symbol-link\n"); /* ��X���ܸ�T */
	
	if((fd = open("symbol_link", O_RDWR)) == -1){ /* �z�L�Ÿ��챵�}�Ҹӥت��ɮ� */
		perror("fail to open");
		exit(1);
	}

	if((n = read(fd, buf, MAX)) == -1){ /* Ū�ت��ɮת����e */
		perror("fail to read");
		exit(1);
	}
	buf[n] = '\0';

	printf("file content is : %s\n", buf); /* �C�L�ت��ɮת����e */

	strcat(buf, ", admin");

	if(write(fd, buf, strlen(buf)) == -1){ /* �V�ت��ɮ׿�X */
		perror("fail to write");
		exit(1);
	}

	printf("done\n"); /* ��X���ܸ�T */

	close(fd);

	return 0;
}
