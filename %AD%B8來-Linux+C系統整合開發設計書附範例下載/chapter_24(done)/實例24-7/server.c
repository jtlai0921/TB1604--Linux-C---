#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stddef.h>
#include <time.h>

/* �ϥ�/home/admin/connect.socket�o���ɮק@���q�T���M���r�ɮ� */
#define PATH "/home/admin/connect.socket"
/* �p�G�W�L�o�Ӯɶ��A�A�Ⱦ�������N���B�z�Ȥ�ݰ�������s���ШD */
#define STALE 30
#define MAX 1024

/* �B�z��ơA�Ω�N�j�g�r���ഫ���p�g�r�šC�ѼƬ��ݭn�ഫ���r�� */
void my_fun(char * p)
{
	if(p == NULL) /* �Ŧ� */
		return;

	for (; *p != '\0'; p++)
		if(*p >= 'A'&& *p <= 'Z') /* �P�_�r�Ũöi���ഫ�A�]�i�H�ϥήw��� */
			*p = *p -'A'+ 'a';
}

/* ��l�ƨ�ơA�إ߮M���r�A�åB�N�ӮM���r�j�w��@�Ӧa�}�]���w���ɮס^�W
*  ���\�Ǧ^0�A���ѶǦ^-1
*  lfd : ���V�x�s�إߦn���M���r���y�z�Ū��ϰ�
*  path : ���w�ɮת����|�A���ɮץ����O���s�b���A�_�hbind��Ʒ|�X��
*/
int init(int *lfd, char *path)
{
    	int fd, len;
    	struct sockaddr_un un_addr;

    	/* �إߤ@�ӮM���r�A�ϥ�Unix��A��������Ƭy���� */
    	if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
		perror("fail to create socket");
       	return -1;
}
	
	/* �p�G�M���r�ɮפw�g�s�b�h�R���A�_�hunlink��ƥX���A�]�����ˬdunlink��ƪ��Ǧ^�� */
unlink(path);

    	/* �]�w�a�}���c�A�Ӧa�}���A��IP�a�}�M��������զ��A�ӬO�@���ɮ׸��| */
    	memset(&un_addr, 0, sizeof(un_addr));
    	un_addr.sun_family = AF_UNIX; /* �ϥ�Unix��a�}�� */
    	strcpy(un_addr.sun_path, path); /* �ƻs�M���r�ɮת����| */

    	len = offsetof(struct sockaddr_un, sun_path) + strlen(path); /* �p���ɮ׸��|������ */

    /* �N�M���r�j�w��@�Ӧa�}���c�W�A�ӮM���r�i�H�}�l�q�T */
    	if (bind(fd, (struct sockaddr *)&un_addr, len) == -1){
        	perror("fail to bind");
goto err;
    	}

	/* �}�l��ť�A�̤j�s���Ƭ�10 */
    	if (listen(fd, 10) == -1){
        	perror("fail to listen");
goto err;
    	}

	*lfd = fd; /* �x�s�M���r���ɮ״y�z�� */
	return 0; /* �����o�̡A���`�Ǧ^ */

err:
    	close(fd); /* �X���A�����M���r */
return -1; /* �Ǧ^-1 */
}

int main(void)
{
int lfd, cfd;
time_t staletime; /* ���իȤ�ݰ�������ɶ� */
    	struct sockaddr_un un_addr;
struct stat statbuf;
char buf[MAX];
	int len, n;

if(init(&lfd, PATH) == -1) /* �I�s��l�ƨ�ơA�إߺ�ť�M���r�A�åB�}�l��ť */
	exit(1);
	
while(1){/* �A�Ⱦ��{���h�b�O�L�a�j�� */
		len = sizeof(struct sockaddr_un);
		
		/* �B�z�@�ӳs���A�إ߳s���M���r�A�o��Ȥ�ݰ�������M���r�ɮ׸��|
* �ñN���x�s�bun_addr���c���A�`�N�o�̨ϥ�sockaddr_un���c���j�p
*/
    		if ((cfd = accept(lfd, (struct sockaddr *)&un_addr, &len)) == -1){
			perror("fail to accept");
        		exit(1);     
		}

    		/* �o��Ȥ���ɮת����|�A�åB�]�w������ */
    		len -= offsetof(struct sockaddr_un, sun_path);
    		un_addr.sun_path[len] = '\0';

		/* �o���ɮ׷R�A�����A��T�A���F���ҫȤ�ݰ�������q�T�ɶ�
* �Ȥ�ݰ�����p�G�����S���ק�q�T�Ϊ��M���r�ɮסA�����ӫȤ�ݦ��i��w�g�����q�T
* �U�����O�����ɮת��v���M�ק�ɶ��A�o�Ǿާ@�ä��O������
* ���O�X��{��������ʦҼ{�A�o�Ǿާ@�٬O���n��
*/
    		if (stat(un_addr.sun_path, &statbuf) == -1) {
        		perror("fail to get status");
			exit(1);
    		}

		/* �ˬd�ɮת��v���A�q�T�Ϊ��M���r�ɮת��v�������O"rwx------"
* �]�N�O�����ΩҦ��̨ϥΪ̥i�H��Ū�B�g�M������ɮת��v���A��L�ϥΪ̨S��
* �o����Unix��M���r�u��Ω�P�@�ϥΪ̪�������������q�T
*/
    		if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) || (statbuf.st_mode & S_IRWXU) != S_IRWXU) {
          	printf("wrong permissions\n");
         		exit(1);
    		}
		
		/* �ˬd�M���r�ɮת���s�ɶ��A�W�L�T�Q�������@�s���M�ק�
* �����Ȥ�ݰ�����i��w�g�_�}�F�s���A�����s���M���r�A�����s��
*/
    		staletime = time(NULL) - STALE;
    		if (statbuf.st_atime < staletime || statbuf.st_ctime < staletime || statbuf.st_mtime < staletime){
          	printf("client is too old\n");
          	close(cfd);
			break;
    		}

		/* �R���Ȥ�ݪ��M���r�ɮ�
* �ӮM���r�ɮץѫȤ�ݰ�����b�I�sbind��ƶi��M���r�j�w���ɭԲ���
*/
    		if(unlink(un_addr.sun_path) == -1){
			perror("fail to unlink");
			exit(1);
		}
         
		my_fun(buf); /* �I�s�j�p�g�ഫ��� */
if(write(cfd, buf, n) == -1){ /* �N�ഫ�᪺�r��o���Ȥ�ݰ���� */
          	perror("fail to write");
			exit(1);
		}
		
		close(cfd); /* �q�T�����A�����M���r�A�ǳƤU�@���q�T */
	}

	/* �R���A�Ⱦ���������M���r�ɮ� */
	if(unlink(PATH) == -1){
		perror("fail to unlink");
		exit(1);
	}
	close(lfd);
	return 0;
}
