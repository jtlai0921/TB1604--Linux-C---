#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

/* ʹ��/home/admin/connect.socket����ļ���Ϊͨ�ŵ��׽����ļ� */
#define PATH "/home/admin/connect.socket"
/* �ͻ��˵��׽����ļ�·����Ŀ¼���ļ���Ϊ���̵Ľ���ID */
#define C_PATH "/home/admin/"
#define MAX 1024

int main(void)
{
	int	cfd, len;
    struct sockaddr_un un_addr;
	char buf[MAX];

    	if ((cfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){ /* ����һ������ */
		perror("fail to create socket");
		exit(1);
	}

    	/* ���ÿͻ��˽���ʹ�õ��׽��ֵ�·�������׽��ֵ��� */
    	memset(&un_addr, 0, sizeof(struct sockaddr_un));
    	un_addr.sun_family = AF_UNIX;
	/* �ͻ��˵��׽����ļ���ΪĬ��Ŀ¼ + ����ID */
    	sprintf(un_addr.sun_path, "%s%d", C_PATH, getpid());
    	len = offsetof(struct sockaddr_un, sun_path) + strlen(un_addr.sun_path);

	/* ����׽����ļ��Ѿ�������ɾ��������unlink����������˲����unlink�����ķ���ֵ */
    	unlink(un_addr.sun_path);

	/* �󶨿ͻ����׽����ļ������ļ��ɷ������˽��̸���ɾ�� */
	if (bind(cfd, (struct sockaddr *)&un_addr, len) == -1){
        	perror("fail to bind");
		exit(1);
    	}
	
	/* �ı��׽����ļ���Ȩ��Ϊrwx------ */
    	if (chmod(un_addr.sun_path, S_IRWXU) < 0) {
         	perror("fail to change model");
		exit(1);
    	}
   
    	memset(&un_addr, 0, sizeof(struct sockaddr_un));
    	un_addr.sun_family = AF_UNIX;
    	strcpy(un_addr.sun_path, PATH); /* �������׽����ļ���·�� */

    	len = offsetof(struct sockaddr_un, sun_path) + strlen(buf);

	/* ʹ�÷��������׽����ļ��������� */
    	if(connect(cfd, (struct sockaddr *)&un_addr, len) < 0) {
        	perror("fail to connect");
		exit(1);
}

strcpy(buf, "CHina");
if(write(cfd, buf, strlen(buf) + 1) == -1){ /* �����ַ������ô�������\0�������� */
perror("fail to write");
		exit(1);
	}

/* ��ȡ���������򷢻صĴ�*/
if(read(cfd, buf, MAX) == -1){
		perror("fail to read");
exit(1);
}
     
printf("recive from server: %s\n", buf); /* ��ӡ�ô� */
     close(cfd);

	return 0;
}
