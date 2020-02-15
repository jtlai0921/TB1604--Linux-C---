#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 1024

int main(int argc, char *argv[ ])
{
	int sfd, dfd;
	struct stat statbuf;
	char buf[MAX];
	int n;

	if(stat(argv[0], &statbuf) == -1){ /* ���o����{���ɮת����A��T */
		perror("fail to stat");
		exit(1);
	}
	
	if(statbuf.st_mode & S_ISUID != 0) /* ���ղըϥΪ̬O�_�]�w�F"�]�w�ϥΪ�ID"�� */
		printf("set user id\n");
else{
		printf("can�zt write to root.txt\n");
		exit(1);
	}

if((sfd = open("test.txt", O_RDONLY)) == -1){ /* �}�ҷ��ɮסA�̭��ѻݭn�g��root.txt�������e */
		perror("fail to open");
		exit(1);
	}

if((dfd = open("root.txt", O_WRONLY)) == -1){ /* �}��root.txt�ɮ� */
		perror("fail to open");
		exit(1);
	}
	
	while((n = read(sfd, buf, MAX)) >0){ /* �Nsfd�ɮפ������e�g��dfd�ɮפ��h */
		if(write(dfd, buf, n) == -1){
			perror("fail to write");
			exit(1);
		}
	}

	if(n < 0){ /* �p�G�]��read��ƶǦ^-1�Ӹ��X�`���A�h����read��ƥX�� */
		perror("fail to read");
		exit(1);
	}

	printf("done\n"); /* ��X���ܸ�T */

	return 0;
}
