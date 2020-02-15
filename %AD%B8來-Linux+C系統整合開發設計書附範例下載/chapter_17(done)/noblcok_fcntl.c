#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX 100000
#define LEN 1024

int main(int argc, char *argv[ ])
{
	int fd1, fd2; 
	FILE *fp;
	char buf[MAX]; /* �j�ɮת��w�İ� */
	int n, rest;
	char *p = buf;
	char content[LEN];
	int flags;

	if(argc != 3){ /* �ʤ��ɮצW */
		printf("expect args\n");
		exit(1);
	}

	fd1 = open(argv[1], O_RDONLY); /* �}�ҿ�J�ɮ� */
	if(fd1 == -1){
		perror("fail to read");
		exit(1);
	}

	fd2 = open(argv[2], O_WRONLY); /* �}�ҿ�X�X����T���ɮ� */
	if(fd2 == -1){
		perror("fail to read");
		exit(1);
	}

	fp = fdopen(fd2, "w"); /* �}���ɮסA�H�u�g���覡 */
	if(fp == NULL){
		perror("fail to open");
		exit(1);
	}

	flags = fcntl(STDOUT_FILENO, F_GETFL, 0); /* �N�зǿ�X�]�w���D����Φ� */
	if(flags == -1){
		perror("fail to fcntl");
		exit(1);
	}
	flags |= O_NONBLOCK; /* �]�w�D����лx */
	if(fcntl(STDOUT_FILENO, F_SETFL, flags) == -1){ /* ���s�]�w�ɮת����A�лx */
		perror("fail to fcntl");
		exit(1);
	}

	rest = read(fd1, buf, MAX); /* Ū�J�ɮ� */
	printf("get %d bytes from %s\n", rest, argv[1]);

	while(rest > 0){ /* ��n��X�����e�٦��Ѿl���~���X */
		errno = 0;
		n = write(STDOUT_FILENO, p, rest); /* ��X�w�İϤ��e */
		
		fprintf(fp, "write %d, errno %s\n", n, strerror(errno)); /* �p�G��X���ѫh��X���~��] */
		if(rest > 0){ /* �p��Ѿl���r�`�� */
			p += n;
			rest -= n;
		}
	}

	printf("done\n");

	close(fd1); /* �����ɮ� */

	fclose(fp);

	return 0;
}
