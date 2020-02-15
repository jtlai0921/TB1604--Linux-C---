#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include "iolib.h" /* �ϥΪ̦ۤv��I/O��Ʈw */

#define MAX_LINE 80
int port = 8000; /*  */

/* �B�z��ơA�Ω�N�j�g�r���ഫ���p�g�r�šC�ѼƬ��ݭn�ഫ���r�� */
void my_fun(char * p)
{
	if(p == NULL) /* �Ŧ� */
		return;

	for (; *p != '\0'; p++)
		if(*p >= 'A'&& *p <= 'Z') /* �P�_�r�Ũöi���ഫ�A�]�i�H�ϥήw��� */
			*p = *p -'A'+ 'a';
}

int main(void)
{
     struct sockaddr_in sin;
     struct sockaddr_in cin;
     int lfd;
     int cfd;
     int sfd;
int rdy;
int client[FD_SETSIZE]; /* �Ȥ�ݳs�����M���r�y�z�Ű}�C */
     int maxi;
     int maxfd; /* �̤j�s���� */
     fd_set rset;
     fd_set allset;
     socklen_t addr_len; /* �a�}���c������ */
     char buf[MAX_LINE];
     char str[INET_ADDRSTRLEN];
     int i;
     int n;
int len;
int opt = 1; /* �M���r�ﶵ */
	char addr_p[18];

     bzero(&sin, sizeof(sin)); /* ��R�a�}���c */
     sin.sin_family = AF_INET;
     sin.sin_addr.s_addr = INADDR_ANY;
     sin.sin_port = htons(port);

	/* �إߤ@�ӭ���s�����M���r */
     lfd = socket(AF_INET, SOCK_STREAM, 0);
     if (lfd == -1) {
          perror("call to socket");
          exit(1);
     }
     
	/* �]�w�M���r�ﶵ�A�ϥιw�]�ﶵ */
setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

/* �j�w�M���r��a�}���c */
     n = bind(lfd, (struct sockaddr *) &sin, sizeof(sin));
     if (n == -1) {
          perror("call to bind");
          exit(1);
}

/* �}�l��ť�s���ШD */
     n = listen(lfd, 20);
     if (n == -1) {
          perror("call to listen"), exit(1);
}

     printf("Accepting connections ...\n"); 

     maxfd = lfd; /* ��̤j�ɮ״y�z�żƶi���l�� */
maxi = -1;

     for (i = 0; i < FD_SETSIZE; i++)  /* ��l�ƫȤ�ݳs���y�z�Ŷ��X */
          client[i] = -1;        

     FD_ZERO(&allset); /* �M���ɮ״y�z�Ŷ��X */
     FD_SET(lfd, &allset); /* �N��ť���r�]�w�b���X�� */

	/* �}�l�A�Ⱦ��{�����L�a�j�� */
     while(1){
         rset = allset;

		/* �o��ثe�i�HŪ���ɮ״y�z�ż� */
         rdy = select(maxfd + 1, &rset, NULL, NULL, NULL);

         if (FD_ISSET(lfd, &rset)){
addr_len = sizeof(cin);

/* �إߤ@�ӳs���y�z�� */
              cfd = accept(lfd, (struct sockaddr *) &cin, &addr_len);
			if(cfd == -1){
				perror("fail to accept");
				exit(1);
			}

/* �M��@�ӪŶ�����m */
              for (i = 0; i < FD_SETSIZE; i++)
                   if (client[i] < 0){
                        client[i] = cfd; /* �N�B�z�ӫȤ�ݪ��s���M���r�]�w��Ӧ�m */
                        break;
}
			
			/* �Ӧh���Ȥ�ݳs���A�A�Ⱦ��ڵ��s���A���X�`�� */
			if (i == FD_SETSIZE){
                   printf("too many clients");
                   exit(1);
}

FD_SET(cfd, &allset); /* �]�w�s�����X */

if (cfd > maxfd) /* �s���s���y�z�� */
                   maxfd = cfd; 
              if (i > maxi)
                   maxi = i;
              if (--rdy <= 0) /* ��֤@�ӳs���y�z�� */
                   continue;
          }

		for (i = 0; i <= maxi; i++){ /* ��C�@�ӳs���y�z�Ű��B�z */
               if ((sfd = client[i]) < 0)
                    continue;
               
if (FD_ISSET(sfd, &rset)){
	
                   n = my_read(sfd, buf, MAX_LINE); /* Ū����� */
                   if (n == 0){
                        printf("the other side has been closed.\n");
                        fflush(stdout); /* ��s���X�׺� */
                        close(sfd);
					
                        FD_CLR(sfd, &allset); /* �M�ųs���y�z�Ű}�C */
                        client[i] = -1;
                   }else{
					/* �N�Ȥ�ݦa�}�ഫ���r�� */
                       	inet_ntop(AF_INET, &cin.sin_addr, addr_p, sizeof(addr_p));    
     
					/* �C�L�Ȥ�ݦa�}�M�q�T�� */
					printf("client IP is %s, port is %d\n", addr_p, ntohs(sin.sin_port));
printf("content is : %s\n", buf); /* �C�L�Ȥ�ݵo�e�L�Ӫ��r�� */

					my_fun(buf); /* �I�s�j�p�g�ഫ��� */
					
                       n = my_write(sfd, buf, len + 1);
					if(n == -1) /* �g��ƥX�� */
						exit(1);
}

                    if (--rdy <= 0)
                         break;      /* �p�G�S���i�HŪ���M���r�h�h�X�`�� */
               }
          }
}

close(lfd); /* �����챵�M���r */

return 0;
}
