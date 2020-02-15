#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LINE 128
#define FILE_SIZE 1024

int main(void)
{
	FILE *in, out;
	FILE *fp;
	struct stat statbuf;
	char file_name[MAX_LINE];
	char *buf;
	int n;
	char *p;
	int file_size = 0;
	int letter, number, blank;

	if(system("ls > temp.txt") == -1){ 
		perror("fail to exec command");
		exit(1);
	}

	fp = fopen("temp.txt", "rb"); 
	if(fp == NULL){
		perror("fail to open");
		exit(1);
	}

	out = fopen("res.txt", "wb"); 
	if(out == NULL){
		perror("fail to open");
		exit(1);
	}


	while(fgets(file_name, MAX_LINE, fp) != NULL){ 
		if(stat(file_name, &statbuf) == -1){ 
			perror("fail to get stat");
			exit(1);
		}

		if(!S_ISDIR(statbuf.st_mode)){ 
			continue;

		if((file_size = statbuf.st_size) > FILE_SIZE)
			file_size = FILE_SIZE;
		file_size++; 
		buf = (char *)malloc(sizeof(char) *file_size);
	
		in = fopen(file_name, "rb"); 
		if(in == NULL){
			perror("fail to open");
			exit(1);
		}

		letter = 0;
		number = 0;
		blank = 0;
	
		while((n = fread(buf, sizeof(char), file_size - 1, in)) > 0){
			buf[n] = '\0';
			p = buf;

			while(*p != '\0'){
				if(('a' <= *p && *p <= 'z') || ('A' <= *p && *p <= 'Z')) 
					letter++;
				if(*p == ' ')
					blank++;
				if('0' <= *p && *p <= '9') 
					number++;
				p++;
			}
		}

		if(n == -1){ 
			perror("fail to read");
			exit(1);
		}

		/* ��X���G�A�r���ơB�Ʀr�ƩM�Ů�� */
		fprintf("%s, he sum of letter is : %d, the sum of number is : %d, 
			the sum of blank is : %d", file_name, letter, number, blank);

		fclose(in);
		free(buf); /* �ݭn����w�İϡA�U�@���|���t�s���w�İ� */
	}

	fclose(fp);
	fclose(out);

	if(unlink("temp.txt") == -1){ /* �R���x�s�{���ɮת�temp.txt�ɮ� */
		perror("fail to unlink");
		exit(1);
	}

	return 0;
}