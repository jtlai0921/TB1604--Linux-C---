#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* ���ըϥΪ����c��C�̭�����Ӧ����ܼ� */
struct a{
	int b;
	int c;	
}r3; /* �w�q�����ܼ�r3 */

/* �Ĥ@�ӽu�{�ϥΧ����ܼ��x�s�h�X��T���c */
void * tfn1(void * arg)
{
	struct a r1;	
	printf("the first one\n "); /* ��X���ܸ�T */
	
	r1.b = 10; /* �]�w�h�X��T���c */
	r1.c = 11;
	
	return (void *)(&r1); /* �Ǧ^�h�X��T���c�����a�} */
}

/* �ĤG�Ӱ�����ϥΰʺA���t�O���骺�覡�x�s�h�X��T���c */
void * tfn2(void * arg)
{
	struct a * r2; 
	
	printf("the second one\n ");  /* ��X���ܸ�T */
	
	r2 =(struct a *) malloc(sizeof(struct a)); /* �ϥΰʺA���t���O���� */
	
	printf("structure at %x\n", r2);
	
	r2->b = 10; /* �]�w�h�X��T */
	r2->c = 11;
	
	return (void *)r2; /* �Ǧ^�h�X���c�����a�} */
}

/* �ĤT�Ӱ�����ϥΥ����ܼ��x�s�h�X��T���c */
void * tfn3(void * arg)
{
	printf("the third one\n:"); /* ��X���ܸ�T */
	
	r3.b = 10; /* �]�w�h�X��T���c */
	r3.c = 11;
	
	return (void *)(&r3); /*�Ǧ^�h�X���c�����a�}*/
}

/* �ĥ|�Ӱ�����ϥ�main��Ƥ��������ܼ��x�s�h�X��T���c */
void * tfn4(void * arg)
{
	struct a *r4 = (struct a *)arg;

	printf("the fourth one\n");  /* ��X���ܸ�T */
	
	r4->b = 10; /* �]�w�h�X��T���c */
	r4->c = 11;
	
	return (void *)(&r4); /*�Ǧ^�h�X���c�����a�}*/
}

int main(void)
{
 	pthread_t tid1, tid2, tid3, tid4;
 	void * res;
 	int err;
 		
	/* ����Ĥ@�ӽu�{ */
 	err = pthread_create(&tid1, NULL, tfn1, NULL); /* �إ߽u�{ */
	if(err != 0){
		printf("can�zt create thread %s\n", strerror(err));
		exit(1);	
	}
	
	err = pthread_join(tid1, &res); /* �o��u�{��������T */
	if(err != 0){
		printf("can�zt join thread %s\n", strerror(err));
		exit(1);	
	}
	printf("1st result: %d, %d\n", ((struct a *)res)->b, ((struct a *)res)->c); /* �C�L�h�X��T */
	
	/* ����ĤG�ӽu�{ */
	err = pthread_create(&tid2, NULL, tfn2, NULL);
	if(err != 0){
		printf("can�zt create thread %s\n", strerror(err));
		exit(1);	
	}
	
	err = pthread_join(tid2, &res); /* �o��u�{��������T */
	if(err != 0){
		printf("can�zt join thread %s\n", strerror(err));
		exit(1);	
	}
	printf("2nd result: %d, %d\n", ((struct a *)res)->b, ((struct a *)res)->c); /* ��X���浲�G */
	free(res); /* ����res�ҫ��V���O����ϰ� */

	/* ����ĤT�ӽu�{ */
	err = pthread_create(&tid3, NULL, tfn3, NULL);
	if(err != 0){
		printf("can�zt create thread %s\n", strerror(err));
		exit(1);	
	}
	
	err = pthread_join(tid3, &res); /* �o��u�{��������T */
	if(err != 0){
		printf("can�zt join thread %s\n", strerror(err));
		exit(1);	
	}
	printf("3rd result: %d, %d\n", ((struct a *)res)->b, ((struct a *)res)->c); /* ��X���浲�G */

	/* ����ĥ|�ӽu�{ */
	err = pthread_create(&tid4, NULL, tfn2, NULL);
	if(err != 0){
		printf("can�zt create thread %s\n", strerror(err));
		exit(1);	
	}
	
	err = pthread_join(tid4, &res); /* �o��u�{��������T */
	if(err != 0){
		printf("can�zt join thread %s\n", strerror(err));
		exit(1);	
	}
	printf("4th result: %d, %d\n", ((struct a *)res)->b, ((struct a *)res)->c); /* ��X���浲�G */
	
 	return 0;
}
