#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 64

/* linux cp���O��²���{�A���O�榡�Gcp des src
* ���\�Ǧ^0�A���ѶǦ^-1�A���ѭ�]�x�s�berrno��
* argv[1] : �ؼ��ɮצWdes�]���Ҩϥε�����|�^
* argv[2]: ���ɮצWsrc�]���Ҩϥε�����|�^
*/
int main(int argc, char* argv[])
{
	char buf[MAX];
	int in, out; /* ��J�ɮשM��X�ɮ� */
	int n;

	if(argc != 3)
		exit(1);

	if((in = open(argv[2], O_RDONLY)) == -1) {/* ���ɮסA"�uŪ"�}�� */
		perror("fail to open");
		exit(1);
	}
	
	/* �ؼ��ɮסA���ɮפ��s�b�h�إߡA���ɮצs�b�h�л\�B�u�g�}�� */
	if((out = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT)) == -1){ 
		perror("fail to open");
		exit(1);
	}
	
	while((n = read(in, buf, MAX)) > 0) /* Ū�J�ɮ� */
		if(write(out, buf, n) != n){ /* ��ڼg�X�r�`�Ƥ�����n�A�g�X�� */
			perror("fail to write");
			exit(1);
		}
	
	if(n < 0){ /* Ū�J�X�� */
		perror("fail to read");
		exit(1);
	}

	printf("copy done\n"); /* ��X���ܸ�T */

	close(in); /* ��������ɮ� */
	close(out);

	return 0;
}
