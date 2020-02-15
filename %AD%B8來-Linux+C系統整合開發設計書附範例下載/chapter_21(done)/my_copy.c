#include <stdio.h>
#include <stdlib.h>

/* �{���ϥΤ�k�Gcopy src des
*  ���{���å�����J���X���B�z�A�Ҧp�ɮ׸��|�����T�ʥH�Υؼ��ɮפw�g�O�_�s�b��
*/
int main(int argc, char *argv[ ])
{
	FILE *fp1, *fp2; /* ���ɮשM�ؼ��ɮ� */
	char buf[1024];
	int n;

	if(argc != 3){ /* �ˬd�ѼƭӼ� */
		printf("wrong command\n");
		exit(1);
	}
	
	if((fp1 = fopen(argv[1], "rb")) == NULL){ /* �}�ҷ��ɮ� */
		perror("fail to open");
		exit(1);
	}
	if((fp2 = fopen(argv[2], "wb")) == NULL){ /* �}�ҥؼ��ɮ� */
		perror("fail to open");
		exit(1);
	}
	
/* �}�l�ƻs�ɮסA�ɮץi��ܤj�A�w�Ĥ@���ˤ��U�A�ҥH�ϥΤ@�Ӵ`���i��Ū�g */
	while((n = fread(buf, sizeof(char), 1024, fp1)) > 0){ /* Ū���ɮסA����N�ɮפ��e����Ū�� */
		if(fwrite(buf, sizeof(char), n, fp2) == -1){ /* �NŪ�X�����e�����g��ؼ��ɮפ��h */
			perror("fail to write");
			exit(1);
		}
	}
	if(n == -1){ /* �p�G����Ū�J�r�`�p��0�Ӹ��X�`���h�����X���F */
		perror("fail to read");
		exit(1);
	}

	fclose(fp1); /* �������ɮשM�ؼ��ɮ� */
	fclose(fp2);

	return 0;
}
