#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_ITEM 3 /* �C���̦h���T�Ӥu�@ */

typedef struct job * Job;

/* �쵲��C���I���c */
struct job{
	pthread_t tid; /* �u�{ID */
	Job next; /* �U�@���쵲��C���I */
	int val; /* ���I�� */
};

pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER; /* �����ܼ��� */

int insert(Job *head, int val, pthread_t tid)
{
	Job p, q;

	p = *head; /* �Y���� */

	if(p != NULL){ /* �P�_���쵲��C�����p */
		while(p->next != NULL){
			p = p->next;	
		}	
	}	

	q = (struct job *)malloc(sizeof(struct job)); /* �����I���t�O����Ŷ� */
	if(q == NULL){
		perror("fail to malloc");
		return -1;
	}
			
	q->next = NULL;
	q->val = val;
	q->tid = tid; /* �]�w���I���Ҧ��̡A�u�{1 */
	p->next = q; /* �N���I��J�u�@���C */
	
	if(p == NULL){ /* �]�w�쵲��C�Y���� */
		*head = q;
		return 0;	
	}
	
	p->next = q; /* ���J�춤�C�� */

	return 0;
}

void get_job(Job head, Job task, int *count)
{
	struct job *p, *q;
	pthread_t tid;

	q = head; /* �ѼƬO�u�@���C�Y */
	p = q->next; /* ����p�@��q�����~�A��ӫ��Ц@�P�e�i */
	
	tid = pthread_self();

	while(p != NULL){ /* �K�Q�쵲��C�A�P�_�C�Ӥu�@���I */
		if(tid == p->tid){ /* ����ݩ�ثe�u�{���u�@���I */
			q->next = p->next;
			p->next = task; /* �N�ӵ��I�q��l���u�@���C���K�U */
			task = p; /* ��J��s���ثe�u�{���u�@���C�W�h */
			p = q->next;
			*count++; /* �w���u�@�O���W */
		}else{
			q = p;
			p = p->next;
		}
	}
}

int free_job(Job head)
{
	Job p, q;

	for(p = head; p != NULL; p = p->next){ /* �u�{�h�X������Ҧ����u�@���I */
		q = p;
         p = p->next;
			free(q);
	}

	return 0;
}

void print(Job head)
{
	Job p;
	
	for(p = head; p != NULL; p = p->next) /* ��X���o���u�@�C�� */
		printf("thread %u: %d\n", p->tid, p->val);
}

void * tfn7(void * arg)
{
	int count; /* �C�����o�쵲��C���I�� */
	struct job * task = NULL; /* �ݩ�ثe�u�{���u�@���I�����C�Y */
	pthread_t tid;

	tid = pthread_self(); /* �o��ثe�u�{ID�A�ھڦ��u�{ID�P�_�u�@�O�_�k�ݩ�ثe�u�{ */
	
	count = 0;
	while(count < MAX_ITEM)
		if(pthread_mutex_trylock(&q_lock) == 0){ /* �N���C�[�� */
			get_job((Job) arg, task, &count);
			pthread_mutex_unlock(&q_lock); /* �ˬd�쵲��C�����A������ */
		}

	print((Job) arg);

	if(free_job(task) == -1)
		exit(1);

	return (void *)0;
}

int main(void)
{
	struct job * item; /* �u�@���C�O�@�Ӧ��Y���I���쵲��C */
	pthread_t tid1, tid2;
	int i; 
	int err;
		
	item = (struct job *)malloc(sizeof(struct job)); /* �]�w�Y���I�A�ӵ��I���x�s���ĸ�T */
	item->next = NULL;
	item->val = 0;
	item->tid = -1;
	
	/* �إߨ�ӽu�{�A�o��ӽu�{�|�ھڦۤv���u�{ID�������P���u�@���I */
	if((err = pthread_create(&tid1, NULL, tfn7, item)) != 0){ /* �إ߲Ĥ@�ӽu�{�A�N�u�@���C�Y�@���u�{���ưѼ� */
		printf("fail to create thread %s\n", strerror(err));
		exit(0);	
	}
	if((err = pthread_create(&tid2, NULL, tfn7, item)) != 0){ /* �إ߲ĤG�ӽu�{�A�N�u�@���C�Y�@���u�{���ưѼ�*/
		printf("fail to create thread %s\n", strerror(err));
		exit(0);	
	}

	printf("===the 1st put===\n"); /* �Ĥ@������A�ѥD�b�{�N�u�@���I�]�w��u�@���C�� */

	pthread_mutex_lock(&q_lock); /*���u�@���C */
	for(i = 0; i < 2; i++){ /* �@����⦸�A�C����J��ӵ��I�A�@���ݩ�u�{1�A�@���ݩ�u�{2 */
		if(insert(&item, i, tid1) == -1)
			exit(1);
		if(insert(&item, i + 1, tid2) == -1)
			exit(1);
	}
		
	if(insert(&item, 10, tid1) == -1)
		exit(1);
		
	pthread_mutex_unlock(&q_lock); /* ������A�ثe�u�@���C���Ѥ��Ӥu�@���I�A3���ݩ�u�{1�A2���ݩ�u�{2 */
	 
	sleep(5); 
	/* ��v�A�O�ҽu�{�i�H�����u�@���I�A�b�o�̤���ϥ�pthread_join��ơC
	* �]�����C���u����ӵ��I�ݩ�u�{2�A�������T�ӵ��I�u�{�O���|�h�X���C
	* �ҥHpthread_join��Ʒ|�ɭP���� 
	*/
	
	printf("===the 2nd put===\n"); /* �ĤG����J�A�ثe���C���w�g�S���u�@���I�F */
	
	pthread_mutex_lock(&q_lock); /* �A������C */
	if(insert(&item, 9, tid2) == -1)
		exit(1);
	pthread_mutex_unlock(&q_lock); /* ������ */
	
	/* �o�Ӯɭԥi�H�ϥ�pthred_join��ƤF */
	err = pthread_join(tid1, NULL); /* �o��u�{1���h�X��T */
	if(err != 0){
		printf("can�zt join thread %s\n", strerror(err));
		exit(1);	
	}
	err = pthread_join(tid2, NULL); /* �o��u�{2���h�X��T */
	if(err != 0){
		printf("can�zt join thread %s\n", strerror(err));
		exit(1);	
	}
	
	printf("main thread done\n"); /* ��X���ܸ�T */
	if(item->next == NULL) /* �p�G���C���S���u�@���I�F�A�h��X���ܸ�T�C */
		printf("No job in the queue\n");	
	free(item); /* �����Y���I */

	return 0;
}
