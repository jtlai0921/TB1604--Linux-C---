#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <unistd.h>  
#include <dirent.h>  
#include <fcntl.h>

#define MAX 1024
#define PATH_SIZE 128

int main(void)
{  
	DIR *dir;
    	struct dirent	*entry; 
    	FILE *fp;
	char path[PATH_SIZE];
	char buf[MAX];

	printf("NAME\tPID\n"); /* ��X���Y */

    	if((dir = opendir( "/proc" ) ) == NULL ) { /* �}��/proc�ؿ� */
        	perror("fail to open dir");
			return -1; 
    	}  

	while((entry = readdir( dir ) ) != NULL){  
        	if(entry->d_name[0] == '.') /* ���L�ثe�ؿ��Aproc�ؿ��S�����ؿ� */ 
			continue;
		/* ���L�t�θ�T�ؿ��A�Ҧ���������ؿ������O�Ʀr�A�Өt�θ�T�ؿ��������O�Ʀr */
		if( '0' <= entry->d_name[0] && entry->d_name[0] <= '9')
			continue;
			
		/* �ϥ�sprintf�����ñ����|�A�䤤���%s�|��entry->d_name��ܪ������ID���N */
		sprintf(path, "/proc/%s/task/%s/status", entry->d_name, entry->d_name);
		
		fp = fopen(path, "r"); /* �}���ɮ� */
		if(fp == NULL){
			perror("fail to open");
			exit(1);
		}

		while(fgets(buf, MAX, fp) != NULL){ /* Ū���C�@�� */
			if(buf[0] == 'N'
 			&& buf[1] == 'a'
			&& buf[2] == 'm'
			&& buf[3] == 'e')
				printf("%s\t", &buf[5]); /* ���L'\t'�A��X���A��T */
			
			if(buf[0] == 'P'
 			&& buf[1] == 'i'
			&& buf[2] == 'd'){
				printf("%s\n", &buf[4]); /* ��XPID��N�����`�� */
				break;
			}
		}

		fclose(fp); /* ����stattus�ɮ� */
	}
    	
	closedir( dir ); /* �����ؿ� */
    
	return 0;  
}
