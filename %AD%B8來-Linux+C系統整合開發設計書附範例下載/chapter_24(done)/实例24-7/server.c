#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stddef.h>
#include <time.h>

/* ʹ��/home/admin/connect.socket����ļ���Ϊͨ�ŵ��׽����ļ� */
#define PATH "/home/admin/connect.socket"
/* ����������ʱ�䣬���������̽�������ͻ��˽��̵��������� */
#define STALE 30
#define MAX 1024

/* �����������ڽ���д�ַ�ת��ΪСд�ַ�������Ϊ��Ҫת�����ַ��� */
void my_fun(char * p)
{
	if(p == NULL) /* �մ� */
		return;

	for (; *p != '\0'; p++)
		if(*p >= 'A'&& *p <= 'Z') /* �ж��ַ�������ת����Ҳ����ʹ�ÿ⺯�� */
			*p = *p -'A'+ 'a';
}

/* ��ʼ�������������׽��֣����ҽ����׽��ְ󶨵�һ����ַ��ָ�����ļ�����
*  �ɹ�����0��ʧ�ܷ���-1
*  lfd : ָ�򱣴洴���õ��׽��ֵ�������������
*  path : ָ���ļ���·�������ļ������ǲ����ڵģ�����bind���������
*/
int init(int *lfd, char *path)
{
    	int fd, len;
    	struct sockaddr_un un_addr;

    	/* ����һ���׽��֣�ʹ��Unix������Ϊ���������� */
    	if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
		perror("fail to create socket");
       	return -1;
}
	
	/* ����׽����ļ��Ѿ�������ɾ��������unlink����������˲����unlink�����ķ���ֵ */
unlink(path);

    	/* ���õ�ַ�ṹ���õ�ַ������IP��ַ�ͽ��̺���ɣ�����һ���ļ�·�� */
    	memset(&un_addr, 0, sizeof(un_addr));
    	un_addr.sun_family = AF_UNIX; /* ʹ��Unix���ַ�� */
    	strcpy(un_addr.sun_path, path); /* �����׽����ļ���·�� */

    	len = offsetof(struct sockaddr_un, sun_path) + strlen(path); /* �����ļ�·���ĳ��� */

    /* ���׽��ְ󶨵�һ����ַ�ṹ�ϣ����׽��ֿ��Կ�ʼͨ�� */
    	if (bind(fd, (struct sockaddr *)&un_addr, len) == -1){
        	perror("fail to bind");
goto err;
    	}

	/* ��ʼ���������������Ϊ10 */
    	if (listen(fd, 10) == -1){
        	perror("fail to listen");
goto err;
    	}

	*lfd = fd; /* �����׽��ֵ��ļ������� */
	return 0; /* ִ�е������������ */

err:
    	close(fd); /* �����ر��׽��� */
return -1; /* ����-1 */
}

int main(void)
{
int lfd, cfd;
time_t staletime; /* ���Կͻ��˽��̵�ʱ�� */
    	struct sockaddr_un un_addr;
struct stat statbuf;
char buf[MAX];
	int len, n;

if(init(&lfd, PATH) == -1) /* ���ó�ʼ�����������������׽��֣����ҿ�ʼ���� */
	exit(1);
	
while(1){/* ����������������ѭ�� */
		len = sizeof(struct sockaddr_un);
		
		/* ����һ�����ӣ����������׽��֣��õ��ͻ��˽��̵��׽����ļ�·��
* �����䱣����un_addr�ṹ�У�ע������ʹ��sockaddr_un�ṹ�Ĵ�С
*/
    		if ((cfd = accept(lfd, (struct sockaddr *)&un_addr, &len)) == -1){
			perror("fail to accept");
        		exit(1);     
		}

    		/* �õ��ͻ����ļ���·�����������ý����� */
    		len -= offsetof(struct sockaddr_un, sun_path);
    		un_addr.sun_path[len] = '\0';

		/* �õ��ļ������״̬��Ϣ��Ϊ����֤�ͻ��˽��̵�ͨ��ʱ��
* �ͻ��˽����������û���޸�ͨ���õ��׽����ļ���˵���ÿͻ����п����Ѿ�����ͨ��
* ����ֱ���֤�ļ���Ȩ�޺��޸�ʱ�䣬��Щ���������Ǳ����
* ���ǳ��ڳ���������Կ��ǣ���Щ�������Ǳ�Ҫ��
*/
    		if (stat(un_addr.sun_path, &statbuf) == -1) {
        		perror("fail to get status");
			exit(1);
    		}

		/* ����ļ���Ȩ�ޣ�ͨ���õ��׽����ļ���Ȩ�ޱ�����"rwx------"
* Ҳ����˵֮���������û������ж���д��ִ�и��ļ���Ȩ�ޣ������û�û��
* ��˵��Unix���׽���ֻ������ͬһ�û��Ľ���֮���ͨ��
*/
    		if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) || (statbuf.st_mode & S_IRWXU) != S_IRWXU) {
          	printf("wrong permissions\n");
         		exit(1);
    		}
		
		/* ����׽����ļ��ĸ���ʱ�䣬������ʮ����δ�����ʺ��޸�
* ˵���ͻ��˽��̿����Ѿ��Ͽ������ӣ��ر������׽��֣���������
*/
    		staletime = time(NULL) - STALE;
    		if (statbuf.st_atime < staletime || statbuf.st_ctime < staletime || statbuf.st_mtime < staletime){
          	printf("client is too old\n");
          	close(cfd);
			break;
    		}

		/* ɾ���ͻ��˵��׽����ļ�
* ���׽����ļ��ɿͻ��˽����ڵ���bind���������׽��ְ󶨵�ʱ������
*/
    		if(unlink(un_addr.sun_path) == -1){
			perror("fail to unlink");
			exit(1);
		}
         
		my_fun(buf); /* ���ô�Сдת������ */
if(write(cfd, buf, n) == -1){ /* ��ת������ִ������ͻ��˽��� */
          	perror("fail to write");
			exit(1);
		}
		
		close(cfd); /* ͨѶ�������ر��׽��֣�׼����һ��ͨ�� */
	}

	/* ɾ�����������̵��׽����ļ� */
	if(unlink(PATH) == -1){
		perror("fail to unlink");
		exit(1);
	}
	close(lfd);
	return 0;
}
