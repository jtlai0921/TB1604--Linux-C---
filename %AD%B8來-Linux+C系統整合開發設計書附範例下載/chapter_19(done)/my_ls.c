#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#define MAX 1024

void my_ls(char *root)
{
	DIR * dir;
	struct dirent * ptr;
	int count;
	
	dir = opendir(root); /* �}�ҥؿ� */
	if(dir == NULL){
		perror("fail to open dir");
		exit(1);
	}

	errno = 0;
	while((ptr = readdir(dir)) != NULL){ /* ����Ū���C�@�ӥؿ��� */
		/* ���L".."�M"."��ӥؿ� */
		if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0)
      		continue;
      	printf("%s\t", ptr->d_name); /* �C�L�C�@���ɮת��ɮצW */
		
		count++; /* ����p�� */
		if(count %5 == 0)
			printf("\n");
	}
	
	if(errno != 0){ /* �p�Gerrno�O0�A�h�����ؿ����ؿ����w�g����Ū�� */
		perror("fail to read dir");
		exit(1);
	}
	
	closedir(dir); /* �`�N�@�w�n�����ؿ� */
}

int main(int argc, char * argv[])
{
	if(argc != 2){
		printf("wrong usage\n");
		exit(1);
	}

	my_ls(argv[1]);

	return 0;
}
