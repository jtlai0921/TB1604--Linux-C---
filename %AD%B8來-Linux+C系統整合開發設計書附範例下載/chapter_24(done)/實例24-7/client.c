#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

/* 使用/home/admin/connect.socket這個檔案作為通訊的套接字檔案 */
#define PATH "/home/admin/connect.socket"
/* 客戶端的套接字檔案路徑的目錄，檔案名為執行緒的執行緒ID */
#define C_PATH "/home/admin/"
#define MAX 1024

int main(void)
{
	int	cfd, len;
    struct sockaddr_un un_addr;
	char buf[MAX];

    	if ((cfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){ /* 建立一個連接 */
		perror("fail to create socket");
		exit(1);
	}

    	/* 設定客戶端執行緒使用的套接字的路徑名和套接字的域 */
    	memset(&un_addr, 0, sizeof(struct sockaddr_un));
    	un_addr.sun_family = AF_UNIX;
	/* 客戶端的套接字檔案名為預設目錄 + 執行緒ID */
    	sprintf(un_addr.sun_path, "%s%d", C_PATH, getpid());
    	len = offsetof(struct sockaddr_un, sun_path) + strlen(un_addr.sun_path);

	/* 如果套接字檔案已經存在則刪除，否則unlink函數出錯，因此不檢查unlink函數的傳回值 */
    	unlink(un_addr.sun_path);

	/* 綁定客戶端套接字檔案，該檔案由服務器端執行緒負責刪除 */
	if (bind(cfd, (struct sockaddr *)&un_addr, len) == -1){
        	perror("fail to bind");
		exit(1);
    	}
	
	/* 改變套接字檔案的權限為rwx------ */
    	if (chmod(un_addr.sun_path, S_IRWXU) < 0) {
         	perror("fail to change model");
		exit(1);
    	}
   
    	memset(&un_addr, 0, sizeof(struct sockaddr_un));
    	un_addr.sun_family = AF_UNIX;
    	strcpy(un_addr.sun_path, PATH); /* 服務器套接字檔案的路徑 */

    	len = offsetof(struct sockaddr_un, sun_path) + strlen(buf);

	/* 使用服務器的套接字檔案進行連接 */
    	if(connect(cfd, (struct sockaddr *)&un_addr, len) < 0) {
        	perror("fail to connect");
		exit(1);
}

strcpy(buf, "CHina");
if(write(cfd, buf, strlen(buf) + 1) == -1){ /* 發送字串，該串包含『\0』結束符 */
perror("fail to write");
		exit(1);
	}

/* 讀取服務器程式發回的串*/
if(read(cfd, buf, MAX) == -1){
		perror("fail to read");
exit(1);
}
     
printf("recive from server: %s\n", buf); /* 列印該串 */
     close(cfd);

	return 0;
}
