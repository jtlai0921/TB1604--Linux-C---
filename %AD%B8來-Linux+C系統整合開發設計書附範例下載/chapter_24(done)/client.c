#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include "iolib.h" /* �W�[�ϥΪ̦ۤw��I/O��Ʈw */

#define MAX_LINE 80

int main(int argc, char *argv[])
{
    	struct sockaddr_in sin;
    	int port = 8000; /* �q�T�𸹡A�ϥ�8000�q�T�� */
	int s_fd;
	char buf[MAX_LINE];
    	char *str = "test";
    	char addr_p[INET_ADDRSTRLEN];
    	int n;
    
	if(argc == 2)
		str = argv[1];

	memset(&sin, sizeof(sin), 0); /* �]�w�a�}���c */
    	sin.sin_family = AF_INET;
    	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
    	sin.sin_port = htons(port);
    
    	s_fd = socket(AF_INET, SOCK_DGRAM, 0); /* �إߤ@�Өϥ�UDP��w���M���r */
    	if(s_fd == -1){
		perror("fail to create socket");
         	exit(1);
     }
	
	/* �ϥ�connect��ƻP�A�Ⱦ��i��s���A�s������N�۷��ϥΤ@��TCP���M���r�i��q�T�F */
     n = connect(s_fd, (struct sockaddr *) &sin, sizeof(sin));
     if(n == -1){
          perror("fail to connect");
          exit(1);
     }else
		printf("connection has been established\n");

    	n = my_write(s_fd, str, strlen(str) + 1); /* �o�e�r��A�Ӧ�]�t�y\0�z������ */
	if(n == -1) /* �g�ާ@���ѡA�{���h�X */
		exit(1);
	
	/* Ū���A�Ⱦ��{���o�^����A�ѩ�O�P�@�x�D���q�T�A���s�b���ɪ����D�C
	* ���O�b�u�����������Ҥ��A�n�B�zŪ�ާ@�����ɰ��D�C
	*/
	n = my_read(s_fd, buf, MAX_LINE);
	if(n == -1) /* Ū���ѡA�h�X�{�� */
		exit(1);	
     
	printf("recive from server: %s\n", buf); /* �C�L�Ӧ� */
	
    	if(close(s_fd) == -1){ /* �����M���r�A�����q�T */
     	perror("fail to close");
		exit(1);
	}

	return 0;
}
