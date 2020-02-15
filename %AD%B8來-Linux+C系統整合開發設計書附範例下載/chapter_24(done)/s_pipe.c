#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

#define MAX 64

int main(void)
{
	int sockfd[2]; /* �Ω��x�sUnix��M���r�y�z�Ū��}�C */
	pid_t pid;
	char buf[MAX];
	int n;

	/* �إߤ@�良�R�W��Unix��M���r�A����������Ƭy�覡��
* sockfd[0]��Ū�ݡAsockfd[1]���g��
 */
	if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1){
		perror("fail to create socket");
		exit(1);
	}

	/* �إߤ@�Ӥl������A���l������ϥέ��إߪ����R�W��Unix��M���r�i��q�T */
	pid = fork();

	if(pid < 0){ /* �إߤl������X�� */
		perror("fail to fork");
		exit(1);
	}else if(pid == 0){ /* �l����� */
		close(sockfd[0]); /* ����Ū�ݡA�l������V���������X���e */

		strcpy(buf, "hello parent");
		n = strlen(buf);

		if(write(sockfd[1], buf, n) == -1){ /* �V��������o�e��T */
			perror("fail to write");
exit(1);
		}
		
		printf("the child done\n"); /* ��X���ܸ�T�A������פ���� */
	}else{
		close(sockfd[1]); /* �����g�ݡA�������Ū���l���������X */
		
		if(read(sockfd[0], buf, MAX) == -1){ /* Ū���l������o�e�������������T */
			perror("fail to read");
			exit(1);
		}

		n = strlen(buf);
		buf[n] = '\0'; /* �W�[�����šA�K���X�Ӹ�T */

		printf("recive from child : %s\n", buf); /* ��X�l������o�e�������������T */
		
		if(wait(NULL) == -1){ /* �^���l��������������A */
			perror("fail to wait");
			exit(1);
		}

		printf("the parent done\n"); /* ��X��������������檺���ܸ�T */
	}

	return 0;
}
