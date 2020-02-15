#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

/* �ϥ�/home/admin/connect.socket�o���ɮק@���q�T���M���r�ɮ� */
#define PATH "/home/admin/connect.socket"
/* �Ȥ�ݪ��M���r�ɮ׸��|���ؿ��A�ɮצW��������������ID */
#define C_PATH "/home/admin/"
#define MAX 1024

int main(void)
{
	int	cfd, len;
    struct sockaddr_un un_addr;
	char buf[MAX];

    	if ((cfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){ /* �إߤ@�ӳs�� */
		perror("fail to create socket");
		exit(1);
	}

    	/* �]�w�Ȥ�ݰ�����ϥΪ��M���r�����|�W�M�M���r���� */
    	memset(&un_addr, 0, sizeof(struct sockaddr_un));
    	un_addr.sun_family = AF_UNIX;
	/* �Ȥ�ݪ��M���r�ɮצW���w�]�ؿ� + �����ID */
    	sprintf(un_addr.sun_path, "%s%d", C_PATH, getpid());
    	len = offsetof(struct sockaddr_un, sun_path) + strlen(un_addr.sun_path);

	/* �p�G�M���r�ɮפw�g�s�b�h�R���A�_�hunlink��ƥX���A�]�����ˬdunlink��ƪ��Ǧ^�� */
    	unlink(un_addr.sun_path);

	/* �j�w�Ȥ�ݮM���r�ɮסA���ɮץѪA�Ⱦ��ݰ�����t�d�R�� */
	if (bind(cfd, (struct sockaddr *)&un_addr, len) == -1){
        	perror("fail to bind");
		exit(1);
    	}
	
	/* ���ܮM���r�ɮת��v����rwx------ */
    	if (chmod(un_addr.sun_path, S_IRWXU) < 0) {
         	perror("fail to change model");
		exit(1);
    	}
   
    	memset(&un_addr, 0, sizeof(struct sockaddr_un));
    	un_addr.sun_family = AF_UNIX;
    	strcpy(un_addr.sun_path, PATH); /* �A�Ⱦ��M���r�ɮת����| */

    	len = offsetof(struct sockaddr_un, sun_path) + strlen(buf);

	/* �ϥΪA�Ⱦ����M���r�ɮ׶i��s�� */
    	if(connect(cfd, (struct sockaddr *)&un_addr, len) < 0) {
        	perror("fail to connect");
		exit(1);
}

strcpy(buf, "CHina");
if(write(cfd, buf, strlen(buf) + 1) == -1){ /* �o�e�r��A�Ӧ�]�t�y\0�z������ */
perror("fail to write");
		exit(1);
	}

/* Ū���A�Ⱦ��{���o�^����*/
if(read(cfd, buf, MAX) == -1){
		perror("fail to read");
exit(1);
}
     
printf("recive from server: %s\n", buf); /* �C�L�Ӧ� */
     close(cfd);

	return 0;
}
