#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX 100000
#define LEN 1024  /* �ϥΥ����̬��w�İϪ��j�p */

int main(int argc, char *argv[ ])
{
	int fd1, fd2; 
	FILE *fp;
	char buf[MAX]; /* �j�ɮת��w�İ� */
	int n, rest;
	char *p = buf;
	char content[LEN];

	if(argc != 3){ /* �ʤ��ɮצW */
		printf("expect args\n");
		exit(1);
	}

	fd1 = open(argv[1], O_RDONLY); /* ��J�ɮ� */
	if(fd1 == -1){
		perror("fail to read");
		exit(1);
	}

	fp = fopen(argv[2], "w"); /* ��X���~��]���ɮסA�ϥή榡��I/O */
	if(fp == NULL){
		perror("fail to read");
		exit(1);
	}

	fd2 = open("test.txt", O_WRONLY); /* ��X�ɮסA�C�t�ɮ�test.txt�ɮ� */
	if(fd2 == -1){
		perror("fail to read");
		exit(1);
	}

	rest = read(fd1, buf, MAX); /* Ū�ɮת����e��w�İ� */
	printf("get %d bytes from %s\n", rest, argv[1]);

	while(rest > 0){ /* ��n��X�����e�٦��Ѿl���~���X */
		errno = 0;
		n = write(fd2, p, rest); /* ��X�w�İϤ��e */
		
		fprintf(fp, "write %d, errno %s\n", n, strerror(errno)); /* �p�G��X���ѫh��X���~��] */
		if(rest > 0){ /* �p��Ѿl���r�`�� */
			p += n;
			rest -= n;
		}
	}

	printf("done\n");

	return 0;
}
