#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(void)
{
	struct stat buf; /* �x�s�ɮת��A��T */
	
	if(stat("sl", &buf) == -1){
		perror("fail to stat");
	exit(1);
}
	
	printf("permission : %d\n", buf.st_mode); /* �C�L�ɮ��v���r */
	printf("inode number : %d\n", buf.st_ino); /* �C�Li�`�I�� */
	printf("device number : %d\n", buf.st_dev); /* �C�L�ɮרt�θ˸m�� */
	printf("r-device number : %d\n", buf.st_rdev); /* �C�L�w��˸m�˸m�� */
	printf("link : %d\n", buf.st_nlink); /* �C�L�w�챵�� */
	printf("uid : %d\n", buf.st_uid); /* �C�L�Ҧ��̨ϥΪ�ID */
	printf("gid : %d\n", buf.st_gid); /* �C�L�Ҧ��̲�ID */
	printf("file size : %d\n", buf.st_size); /* �C�L�ɮת��j�p */
	printf("access time : %d\n", buf.st_atime); /* �C�L�̪�@�����s���ɶ�*/
	printf("motify time : %d\n", buf.st_mtime); /* �C�L�̪�@���ק��ɮפ��e���ɶ� */
	printf("change time : %d\n", buf.st_ctime); /* �C�L�̪�@���ק��ɮ��ݩʪ��ɶ� */
	printf("buf size : %d\n", buf.st_blksize); /* �C�L�̾A���w�İϤj�p */
	printf("block size : %d\n", buf.st_blocks); /* �C�L�ɮצb�~�s�W���Ϊ��ж��� */

	if(stat("test.txt", &buf) == -1){
		perror("fail to stat");
	exit(1);
}

	printf("permission : %d\n", buf.st_mode); /* �C�L�ɮ��v���r */
	printf("inode number : %d\n", buf.st_ino); /* �C�Li�`�I�� */
	printf("device number : %d\n", buf.st_dev); /* �C�L�ɮרt�θ˸m�� */
	printf("r-device number : %d\n", buf.st_rdev); /* �C�L�w��˸m�˸m�� */
	printf("link : %d\n", buf.st_nlink); /* �C�L�w�챵�� */
	printf("uid : %d\n", buf.st_uid); /* �C�L�Ҧ��̨ϥΪ�ID */
	printf("gid : %d\n", buf.st_gid); /* �C�L�Ҧ��̲�ID */
	printf("file size : %d\n", buf.st_size); /* �C�L�ɮת��j�p */
	printf("access time : %d\n", buf.st_atime); /* �C�L�̪�@�����s���ɶ�*/
	printf("motify time : %d\n", buf.st_mtime); /* �C�L�̪�@���ק��ɮפ��e���ɶ� */
	printf("change time : %d\n", buf.st_ctime); /* �C�L�̪�@���ק��ɮ��ݩʪ��ɶ� */
	printf("buf size : %d\n", buf.st_blksize); /* �C�L�̾A���w�İϤj�p */
	printf("block size : %d\n", buf.st_blocks); /* �C�L�ɮצb�~�s�W���Ϊ��ж��� */
	
return 0;
}
