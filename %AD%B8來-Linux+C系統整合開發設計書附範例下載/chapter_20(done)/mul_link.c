#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAX 1024

int main(void)
{
	int fd;
	char buf[MAX];
	int n;

if(symlink("test.txt", "sl") == -1){ /* �إߤ@�ӲŸ��챵�A���Vtest.txt�ɮ� */
		perror("fail to create symbol link");
		exit(1);
	}
	
	/* �A���إߤ@�ӲŸ��챵�A���Vsl�C�N���Φ��F�@�ӲŸ��챵��Gsl2->sl->test.txt
	* test.txt�ɮ׬O�ت��ɮסA�]�O�ӲŸ��챵�쪺���I 
	*/
if(symlink("sl", "sl2") == -1){ 
		perror("fail to create symbol link");
		exit(1);
	}
		
	if((fd = open("sl2", O_RDWR)) == -1){ /* �}�Ҫ���ڤW�O�ت��ɮ�test.txt */
		perror("fail to open");
		exit(1);
	}

	printf("already create symbol-links\n");

	if((n = read(fd, buf, MAX)) == -1){ /* Ū�ɮת����e */
		perror("fail to read");
		exit(1);
	}
	buf[n] = '\0';

	printf("file content is : %s\n", buf); /* ��X���G*/

	strcat(buf, ", admin");

	if(write(fd, buf, strlen(buf)) == -1){ /* �V���ɮ׼g���e */
		perror("fail to write");
		exit(1);
	}

	printf("done\n"); /* ��X���ܸ�T */

	close(fd);

	return 0;
}
