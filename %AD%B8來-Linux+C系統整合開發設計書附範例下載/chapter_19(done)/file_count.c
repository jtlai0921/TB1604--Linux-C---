#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#define MAX 1024

/* get_file_count��ƪ��ѼƬO�@�Ӹ��|�A��ܥؿ��𪺮ڡC
* �Ө�ƶǦ^�Ѽƫ��w�ؿ��U�ҥδ��q�ɮת��ƥءC
*/
int get_file_count(char *root)
{
	DIR * dir;
	struct dirent * ptr;
	int total = 0; /* �ɮ��`�� */
	char path[MAX];
	
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
      	if(ptr->d_type == DT_DIR){ /* �p�G�O�ؿ��A�h���k�I�sget_file_count��� */
      		sprintf(path, "%s/%s", root, ptr->d_name);
      		total += get_file_count(path);
      	}
      	if(ptr->d_type == DT_REG) /* �p�G�O���q�ɮסA�h�`�ƻ��W */
      		total++;	
	}

	if(errno != 0){ /* �p�Gerrno�O0�A�h�����ؿ����ؿ����w�g����Ū�� */
		perror("fail to read dir");
		exit(1);
	}
	
	closedir(dir); /* �`�N�@�w�n�����ؿ� */

	return total; /* �Ǧ^���q�ɮ׼� */
}

int main(int argc, char * argv[])
{
	int total;
	
	if(argc != 2){ /* ���O��Ѽƿ��~ */
		printf("wrong usage\n");
		exit(1);
	}

	total = get_file_count(argv[1]); /* �o���ɮ׼ƥ� */

	printf("%s has %d files\n", argv[1], total);

	return 0;
}
