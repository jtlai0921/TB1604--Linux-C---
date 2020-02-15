#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

int main(void)
{
	DIR *dir;
	struct dirent * d_ptr;

	dir=opendir("/home/admin"); /* �}�ҥؿ� */
	if(dir == NULL){
		perror("fail to open dir");
		exit(1);
	}
	
	while((d_ptr = readdir(dir))!=NULL){ /* ���ǿ�X�ؿ������C���ɮת��ɮצW */
		printf("dir name is :%s \n",d_ptr->d_name);
	}

	printf("read again\n");
	rewinddir(dir); /* �ؿ��ɮת�Ū�g��m�k�s */
	
	while((d_ptr = readdir(dir))!=NULL){ /* �A����X�C���ɮ� */
		printf("dir name is :%s\n",d_ptr->d_name);
	}

	closedir(dir); /* �����ؿ� */

	return 0;
}
