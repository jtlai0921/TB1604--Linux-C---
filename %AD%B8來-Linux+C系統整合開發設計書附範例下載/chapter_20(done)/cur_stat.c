#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX 1024
#define PID 6
#define PATH_SIZE 128

int main(void)
{
	FILE *fp;
	pid_t pid;
	char pid_str[PID];
	char path[PATH_SIZE];
	char buf[MAX];

	pid = getpid(); /* �o������ID */
	sprintf(pid_str, "%d", pid); /* �N�����ID�ѼƦr�ഫ���r�� */
	strcpy(path, "/proc/self/task/"); /* �������|�A�}��"/proc/self/task/�����ID"�ؿ��U��status�ɮ� */
	strcat(path, pid_str);
	strcat(path, "/status");

	fp = fopen(path, "r"); /* �}�Ҹ��ɮסA�H�uŪ���覡�}�� */
	if(fp == NULL){
		perror("fail to open");
		exit(1);
	}

	while(fgets(buf, MAX, fp) != NULL) /* ����Ū���C�@��A�åB�C�L */
		printf("%s", buf);

	fclose(fp); /* �����ɮ� */

	return 0;
}
