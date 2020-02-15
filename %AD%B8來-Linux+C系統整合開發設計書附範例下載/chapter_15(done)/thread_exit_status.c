#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* 測試使用的結構體。裡面有兩個成員變數 */
struct a{
	int b;
	int c;	
}r3; /* 定義全局變數r3 */

/* 第一個線程使用局部變數儲存退出資訊結構 */
void * tfn1(void * arg)
{
	struct a r1;	
	printf("the first one\n "); /* 輸出提示資訊 */
	
	r1.b = 10; /* 設定退出資訊結構 */
	r1.c = 11;
	
	return (void *)(&r1); /* 傳回退出資訊結構的首地址 */
}

/* 第二個執行緒使用動態分配記憶體的方式儲存退出資訊結構 */
void * tfn2(void * arg)
{
	struct a * r2; 
	
	printf("the second one\n ");  /* 輸出提示資訊 */
	
	r2 =(struct a *) malloc(sizeof(struct a)); /* 使用動態分配的記憶體 */
	
	printf("structure at %x\n", r2);
	
	r2->b = 10; /* 設定退出資訊 */
	r2->c = 11;
	
	return (void *)r2; /* 傳回退出結構的首地址 */
}

/* 第三個執行緒使用全局變數儲存退出資訊結構 */
void * tfn3(void * arg)
{
	printf("the third one\n:"); /* 輸出提示資訊 */
	
	r3.b = 10; /* 設定退出資訊結構 */
	r3.c = 11;
	
	return (void *)(&r3); /*傳回退出結構的首地址*/
}

/* 第四個執行緒使用main函數中的局部變數儲存退出資訊結構 */
void * tfn4(void * arg)
{
	struct a *r4 = (struct a *)arg;

	printf("the fourth one\n");  /* 輸出提示資訊 */
	
	r4->b = 10; /* 設定退出資訊結構 */
	r4->c = 11;
	
	return (void *)(&r4); /*傳回退出結構的首地址*/
}

int main(void)
{
 	pthread_t tid1, tid2, tid3, tid4;
 	void * res;
 	int err;
 		
	/* 執行第一個線程 */
 	err = pthread_create(&tid1, NULL, tfn1, NULL); /* 建立線程 */
	if(err != 0){
		printf("can』t create thread %s\n", strerror(err));
		exit(1);	
	}
	
	err = pthread_join(tid1, &res); /* 得到線程的結束資訊 */
	if(err != 0){
		printf("can』t join thread %s\n", strerror(err));
		exit(1);	
	}
	printf("1st result: %d, %d\n", ((struct a *)res)->b, ((struct a *)res)->c); /* 列印退出資訊 */
	
	/* 執行第二個線程 */
	err = pthread_create(&tid2, NULL, tfn2, NULL);
	if(err != 0){
		printf("can』t create thread %s\n", strerror(err));
		exit(1);	
	}
	
	err = pthread_join(tid2, &res); /* 得到線程的結束資訊 */
	if(err != 0){
		printf("can』t join thread %s\n", strerror(err));
		exit(1);	
	}
	printf("2nd result: %d, %d\n", ((struct a *)res)->b, ((struct a *)res)->c); /* 輸出執行結果 */
	free(res); /* 釋放res所指向的記憶體區域 */

	/* 執行第三個線程 */
	err = pthread_create(&tid3, NULL, tfn3, NULL);
	if(err != 0){
		printf("can』t create thread %s\n", strerror(err));
		exit(1);	
	}
	
	err = pthread_join(tid3, &res); /* 得到線程的結束資訊 */
	if(err != 0){
		printf("can』t join thread %s\n", strerror(err));
		exit(1);	
	}
	printf("3rd result: %d, %d\n", ((struct a *)res)->b, ((struct a *)res)->c); /* 輸出執行結果 */

	/* 執行第四個線程 */
	err = pthread_create(&tid4, NULL, tfn2, NULL);
	if(err != 0){
		printf("can』t create thread %s\n", strerror(err));
		exit(1);	
	}
	
	err = pthread_join(tid4, &res); /* 得到線程的結束資訊 */
	if(err != 0){
		printf("can』t join thread %s\n", strerror(err));
		exit(1);	
	}
	printf("4th result: %d, %d\n", ((struct a *)res)->b, ((struct a *)res)->c); /* 輸出執行結果 */
	
 	return 0;
}
