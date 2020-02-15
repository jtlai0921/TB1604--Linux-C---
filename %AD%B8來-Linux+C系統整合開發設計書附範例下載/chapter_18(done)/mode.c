#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char * argv[])
{
	char *str;
	int i;
	struct stat buf; /* �x�s�ɮת��A��T */
	
	for(i = 1; i < argc; i++){
		printf("%s ", argv[i]);

		if(stat(argv[i], &buf) == -1){ /* �o���ɮת����A */
			perror("fail to stat");
			continue;
		}

		if(S_ISREG(buf.st_mode)) /* ���q�ɮ� */
			printf("regular file \n");
		else if(S_ISDIR(buf.st_mode)) /* �ؿ��ɮ� */
			printf( "directory file\n");
		else if(S_ISCHR(buf.st_mode)) /* �r�ůS���ɮ� */
			printf( "character special file\n");
		else if(S_ISBLK(buf.st_mode)) /* ���S���ɮ� */
			printf( "block special file\n");
		else if(S_ISFIFO(buf.st_mode)) /*�R�W�޽u�ɮ� */
			printf( "FIFO file\n");
		else if(S_ISSOCK(buf.st_mode)) /* �M���r�ɮ� */
			printf( "socket file\n");
		else if(S_ISLNK(buf.st_mode)) /* �Ÿ��챵�ɮ� */
			printf( "symbolic link file\n");
		else /* �������� */
			printf( "unkonw\n");
}
	
return 0;
}
