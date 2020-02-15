#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* Ū�ާ@���X�A�N�Ҧ��̨ϥΪ̡B�ըϥΪ̩M�䥦�ϥΪ̪�Ū�v�������W�[ */
#define READ_MASK S_IRUSR | S_IRGRP | S_IROTH
/* �g�ާ@���X�A�N�Ҧ��̨ϥΪ̡B�ըϥΪ̩M�䥦�ϥΪ̪��g�v�������W�[ */
#define WRITE_MASK S_IWUSR | S_IWGRP | S_IWOTH

int main(void)
{
	int fd;
	struct stat statbuf;

	/* �}�Ҥ@���ɮסA�p�G���ɮפ��s�b�A�h�ϥ�Ū�ާ@���X�@���v���r�إ߸��ɮ� */
	if((fd = open("test.txt", O_RDONLY | O_CREAT, READ_MASK)) == -1){
		perror("fail to open");
		exit(1);
	}
	
	printf("before changing model\n"); /* ��X���ܸ�T */
	 
	if(fstat(fd, &statbuf) == -1){ /* �b�}�Ҫ��ɮפW�o���ɮת��A */
		perror("fail to get status");
		exit(1);	
	}
	
	if(statbuf.st_mode & S_IRUSR) /* �Ҧ��̨ϥΪ̨㦳Ū�ɮת��v�� */
		printf("user can read\n");
	if(statbuf.st_mode & S_IRGRP) /* �ըϥΪ̨㦳Ū�ɮת��v�� */
		printf("group user can read\n");
	if(statbuf.st_mode & S_IROTH) /* �䥦�ϥΪ̨㦳Ū�ɮת��v�� */
		printf("other user can read\n");
	printf("\n");

	/* �ϥμg�ާ@�v���r�����ɮת��v���A
* ���ܫ��ɮת����Ҧ�Ū�v�������A
* ���ӥN�����O�Ҧ����g�v�� 
*/
	if(fchmod(fd, WRITE_MASK) == -1){
		perror("fail to change model");
		exit(1);	
	}
	
	printf("after changing model\n");
	
	if(fstat(fd, &statbuf) == -1){ /* �A���h�ɮת����A */
		perror("fail to get status");
		exit(1);	
	}
	
	printf("check the file by file-descriptor\n");

	/* �����ϥθ��ɮת��y�z�Ũ��o�ɮת��A�A�ˬd�ɮ��v���O�_��s */
	if(statbuf.st_mode & S_IWUSR) /* �Ҧ��̨ϥΪ̨㦳�g�ɮת��v�� */
		printf("user can write\n");
	if(statbuf.st_mode & S_IWGRP) /* �ըϥΪ̨㦳�g�ɮת��v�� */
		printf("group user can write\n");
	if(statbuf.st_mode & S_IWOTH) /* �䥦�ϥΪ̨㦳�g�ɮת��v�� */
		printf("other user can write\n");
printf("\n");
	
	/* �A���q�ϺФW���o���ɮת��ɮת��A�A�ˬd�ϺФW���ɮת��v���O�_�]�w�g��s */
	if(stat("test.txt", &statbuf) == -1){
		perror("fail to get status"); 
		exit(1);	
	}

	printf("check the file in the disk\n");
	
	/* �ϺФW���ɮ��v���]�w�g��s */
	if(statbuf.st_mode & S_IWUSR) /* �Ҧ��̨ϥΪ̨㦳�g�ɮת��v�� */
		printf("user can write\n");
	if(statbuf.st_mode & S_IWGRP) /* �ըϥΪ̨㦳�g�ɮת��v�� */
		printf("group user can write\n");
	if(statbuf.st_mode & S_IWOTH) /* �䥦�ϥΪ̨㦳�g�ɮת��v�� */
		printf("other user can write\n");	
	printf("\n");

	
	sleep(10); /* ��v10���� */
	
	printf("done\n"); /* �C�L���ܸ�T */
	
	close(fd); /* �ɮ������A�Ҧ��w�İϪ����e�R�~��ϺФW */
 
	return 0;
}
