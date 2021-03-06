#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_ITEM 3 /* 每次最多取三個工作 */

typedef struct job * Job;

/* 鏈結串列結點結構 */
struct job{
	pthread_t tid; /* 線程ID */
	Job next; /* 下一個鏈結串列結點 */
	int val; /* 結點值 */
};

pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER; /* 全局變數鎖 */

int insert(Job *head, int val, pthread_t tid)
{
	Job p, q;

	p = *head; /* 頭指標 */

	if(p != NULL){ /* 判斷空鏈結串列的情況 */
		while(p->next != NULL){
			p = p->next;	
		}	
	}	

	q = (struct job *)malloc(sizeof(struct job)); /* 為結點分配記憶體空間 */
	if(q == NULL){
		perror("fail to malloc");
		return -1;
	}
			
	q->next = NULL;
	q->val = val;
	q->tid = tid; /* 設定結點的所有者，線程1 */
	p->next = q; /* 將結點鏈入工作隊列 */
	
	if(p == NULL){ /* 設定鏈結串列頭指標 */
		*head = q;
		return 0;	
	}
	
	p->next = q; /* 插入到隊列中 */

	return 0;
}

void get_job(Job head, Job task, int *count)
{
	struct job *p, *q;
	pthread_t tid;

	q = head; /* 參數是工作隊列頭 */
	p = q->next; /* 指標p作為q的後繼，兩個指標共同前進 */
	
	tid = pthread_self();

	while(p != NULL){ /* 便利鏈結串列，判斷每個工作結點 */
		if(tid == p->tid){ /* 找到屬於目前線程的工作結點 */
			q->next = p->next;
			p->next = task; /* 將該結點從原始的工作隊列中摘下 */
			task = p; /* 鏈入到新的目前線程的工作隊列上去 */
			p = q->next;
			*count++; /* 已取工作是遞增 */
		}else{
			q = p;
			p = p->next;
		}
	}
}

int free_job(Job head)
{
	Job p, q;

	for(p = head; p != NULL; p = p->next){ /* 線程退出時釋放所有的工作結點 */
		q = p;
         p = p->next;
			free(q);
	}

	return 0;
}

void print(Job head)
{
	Job p;
	
	for(p = head; p != NULL; p = p->next) /* 輸出取得的工作列表 */
		printf("thread %u: %d\n", p->tid, p->val);
}

void * tfn7(void * arg)
{
	int count; /* 每次取得鏈結串列結點數 */
	struct job * task = NULL; /* 屬於目前線程的工作結點的隊列頭 */
	pthread_t tid;

	tid = pthread_self(); /* 得到目前線程ID，根據此線程ID判斷工作是否歸屬於目前線程 */
	
	count = 0;
	while(count < MAX_ITEM)
		if(pthread_mutex_trylock(&q_lock) == 0){ /* 將隊列加鎖 */
			get_job((Job) arg, task, &count);
			pthread_mutex_unlock(&q_lock); /* 檢查鏈結串列結束，釋放鎖 */
		}

	print((Job) arg);

	if(free_job(task) == -1)
		exit(1);

	return (void *)0;
}

int main(void)
{
	struct job * item; /* 工作隊列是一個有頭結點的鏈結串列 */
	pthread_t tid1, tid2;
	int i; 
	int err;
		
	item = (struct job *)malloc(sizeof(struct job)); /* 設定頭結點，該結點不儲存有效資訊 */
	item->next = NULL;
	item->val = 0;
	item->tid = -1;
	
	/* 建立兩個線程，這兩個線程會根據自己的線程ID取走不同的工作結點 */
	if((err = pthread_create(&tid1, NULL, tfn7, item)) != 0){ /* 建立第一個線程，將工作隊列頭作為線程體函數參數 */
		printf("fail to create thread %s\n", strerror(err));
		exit(0);	
	}
	if((err = pthread_create(&tid2, NULL, tfn7, item)) != 0){ /* 建立第二個線程，將工作隊列頭作為線程體函數參數*/
		printf("fail to create thread %s\n", strerror(err));
		exit(0);	
	}

	printf("===the 1st put===\n"); /* 第一次執行，由主軸程將工作結點設定到工作隊列中 */

	pthread_mutex_lock(&q_lock); /*鎖住工作隊列 */
	for(i = 0; i < 2; i++){ /* 共執行兩次，每次放入兩個結點，一個屬於線程1，一個屬於線程2 */
		if(insert(&item, i, tid1) == -1)
			exit(1);
		if(insert(&item, i + 1, tid2) == -1)
			exit(1);
	}
		
	if(insert(&item, 10, tid1) == -1)
		exit(1);
		
	pthread_mutex_unlock(&q_lock); /* 釋放鎖，目前工作隊列中由五個工作結點，3個屬於線程1，2個屬於線程2 */
	 
	sleep(5); 
	/* 休眠，保證線程可以取走工作結點，在這裡不能使用pthread_join函數。
	* 因為隊列中只有兩個結點屬於線程2，未取走三個結點線程是不會退出的。
	* 所以pthread_join函數會導致死鎖 
	*/
	
	printf("===the 2nd put===\n"); /* 第二次輸入，目前隊列中已經沒有工作結點了 */
	
	pthread_mutex_lock(&q_lock); /* 再次鎖住隊列 */
	if(insert(&item, 9, tid2) == -1)
		exit(1);
	pthread_mutex_unlock(&q_lock); /* 釋放鎖 */
	
	/* 這個時候可以使用pthred_join函數了 */
	err = pthread_join(tid1, NULL); /* 得到線程1的退出資訊 */
	if(err != 0){
		printf("can』t join thread %s\n", strerror(err));
		exit(1);	
	}
	err = pthread_join(tid2, NULL); /* 得到線程2的退出資訊 */
	if(err != 0){
		printf("can』t join thread %s\n", strerror(err));
		exit(1);	
	}
	
	printf("main thread done\n"); /* 輸出提示資訊 */
	if(item->next == NULL) /* 如果隊列中沒有工作結點了，則輸出提示資訊。 */
		printf("No job in the queue\n");	
	free(item); /* 釋放頭結點 */

	return 0;
}
