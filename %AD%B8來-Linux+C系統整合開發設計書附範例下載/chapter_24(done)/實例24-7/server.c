#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stddef.h>
#include <time.h>

/* 使用/home/admin/connect.socket這個檔案作為通訊的套接字檔案 */
#define PATH "/home/admin/connect.socket"
/* 如果超過這個時間，服務器執行緒將不處理客戶端執行緒的連接請求 */
#define STALE 30
#define MAX 1024

/* 處理函數，用於將大寫字符轉換為小寫字符。參數為需要轉換的字串 */
void my_fun(char * p)
{
	if(p == NULL) /* 空串 */
		return;

	for (; *p != '\0'; p++)
		if(*p >= 'A'&& *p <= 'Z') /* 判斷字符並進行轉換，也可以使用庫函數 */
			*p = *p -'A'+ 'a';
}

/* 初始化函數，建立套接字，並且將該套接字綁定到一個地址（指定的檔案）上
*  成功傳回0，失敗傳回-1
*  lfd : 指向儲存建立好的套接字的描述符的區域
*  path : 指定檔案的路徑，該檔案必須是不存在的，否則bind函數會出錯
*/
int init(int *lfd, char *path)
{
    	int fd, len;
    	struct sockaddr_un un_addr;

    	/* 建立一個套接字，使用Unix域，類型為資料流類型 */
    	if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
		perror("fail to create socket");
       	return -1;
}
	
	/* 如果套接字檔案已經存在則刪除，否則unlink函數出錯，因此不檢查unlink函數的傳回值 */
unlink(path);

    	/* 設定地址結構，該地址不再由IP地址和執行緒號組成，而是一個檔案路徑 */
    	memset(&un_addr, 0, sizeof(un_addr));
    	un_addr.sun_family = AF_UNIX; /* 使用Unix域地址族 */
    	strcpy(un_addr.sun_path, path); /* 複製套接字檔案的路徑 */

    	len = offsetof(struct sockaddr_un, sun_path) + strlen(path); /* 計算檔案路徑的長度 */

    /* 將套接字綁定到一個地址結構上，該套接字可以開始通訊 */
    	if (bind(fd, (struct sockaddr *)&un_addr, len) == -1){
        	perror("fail to bind");
goto err;
    	}

	/* 開始監聽，最大連接數為10 */
    	if (listen(fd, 10) == -1){
        	perror("fail to listen");
goto err;
    	}

	*lfd = fd; /* 儲存套接字的檔案描述符 */
	return 0; /* 執行到這裡，正常傳回 */

err:
    	close(fd); /* 出錯，關閉套接字 */
return -1; /* 傳回-1 */
}

int main(void)
{
int lfd, cfd;
time_t staletime; /* 測試客戶端執行緒的時間 */
    	struct sockaddr_un un_addr;
struct stat statbuf;
char buf[MAX];
	int len, n;

if(init(&lfd, PATH) == -1) /* 呼叫初始化函數，建立監聽套接字，並且開始監聽 */
	exit(1);
	
while(1){/* 服務器程式多半是無窮迴圈 */
		len = sizeof(struct sockaddr_un);
		
		/* 處理一個連接，建立連接套接字，得到客戶端執行緒的套接字檔案路徑
* 並將其儲存在un_addr結構中，注意這裡使用sockaddr_un結構的大小
*/
    		if ((cfd = accept(lfd, (struct sockaddr *)&un_addr, &len)) == -1){
			perror("fail to accept");
        		exit(1);     
		}

    		/* 得到客戶端檔案的路徑，並且設定結束符 */
    		len -= offsetof(struct sockaddr_un, sun_path);
    		un_addr.sun_path[len] = '\0';

		/* 得到檔案愛你的狀態資訊，為了驗證客戶端執行緒的通訊時間
* 客戶端執行緒如果長期沒有修改通訊用的套接字檔案，說明該客戶端有可能已經結束通訊
* 下面分別驗證檔案的權限和修改時間，這些操作並不是必須的
* 但是出於程式的完整性考慮，這些操作還是必要的
*/
    		if (stat(un_addr.sun_path, &statbuf) == -1) {
        		perror("fail to get status");
			exit(1);
    		}

		/* 檢查檔案的權限，通訊用的套接字檔案的權限必須是"rwx------"
* 也就是說之用所有者使用者可以有讀、寫和執行該檔案的權限，其他使用者沒有
* 這說明Unix域套接字只能用於同一使用者的執行緒之間的通訊
*/
    		if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) || (statbuf.st_mode & S_IRWXU) != S_IRWXU) {
          	printf("wrong permissions\n");
         		exit(1);
    		}
		
		/* 檢查套接字檔案的更新時間，超過三十秒鐘未作存取和修改
* 說明客戶端執行緒可能已經斷開了連接，關閉連接套接字，結束連接
*/
    		staletime = time(NULL) - STALE;
    		if (statbuf.st_atime < staletime || statbuf.st_ctime < staletime || statbuf.st_mtime < staletime){
          	printf("client is too old\n");
          	close(cfd);
			break;
    		}

		/* 刪除客戶端的套接字檔案
* 該套接字檔案由客戶端執行緒在呼叫bind函數進行套接字綁定的時候產生
*/
    		if(unlink(un_addr.sun_path) == -1){
			perror("fail to unlink");
			exit(1);
		}
         
		my_fun(buf); /* 呼叫大小寫轉換函數 */
if(write(cfd, buf, n) == -1){ /* 將轉換後的字串發給客戶端執行緒 */
          	perror("fail to write");
			exit(1);
		}
		
		close(cfd); /* 通訊結束，關閉套接字，準備下一次通訊 */
	}

	/* 刪除服務器執行緒的套接字檔案 */
	if(unlink(PATH) == -1){
		perror("fail to unlink");
		exit(1);
	}
	close(lfd);
	return 0;
}
