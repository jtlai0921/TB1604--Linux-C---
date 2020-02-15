#include <stdio.h>
#include <stdlib.h>

#define MAX 1024

int main(void)
{
	FILE *fp;
	char buf[MAX];
	int n;
	char *p;
	int letter, number, blank;

	fp = fopen("text.txt", "rb"); /* �}�Ҹ��ɮ� */
	if(fp == NULL){
		perror("fail to open");
		exit(1);
	}

	letter = 0;
	number = 0;
	blank = 0;
	/* �`��Ū���ɮת����e�A���ɮץi��ܤj�A����@��Ū�J��w�İϤ� */
	while((n = fread(buf, sizeof(char), MAX - 1, fp)) > 0){
		buf[n] = '\0'; /* ��ʳ]�w������ */
		p = buf;

		while(*p != '\0'){ /* �B�z�C��Ū�J���ɮפ��e */
			if(('a' <= *p && *p <= 'z') || ('A' <= *p && *p <= 'Z')) /* �P�_���r�� */
				letter++;
			if(*p == ' ') /* �P�_���Ů� */
				blank++;
			if('0' <= *p && *p <= '9') /* �P�_���Ʀr */
				number++;
			p++;
		}
	}

	if(n == -1){ /* Ū�ާ@�X�� */
		perror("fail to read");
		exit(1);
	}

	/* ��X���G�A�r���ơB�Ʀr�ƩM�Ů�� */
	printf("the sum of letter is : %d, the sum of number is : %d, 
		the sum of blank is : %d", letter, number, blank);

	return 0;
}
