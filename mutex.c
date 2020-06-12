#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100

void* thread_inc(void* arg);
void* thread_des(void* arg);

long long num = 0;
pthread_mutex_t mutex;		// mutex에 사용되는 변수

int main(int argc, char* argv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}

	for (i = 0; i < NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);

	printf("result: %lld \n", num);
	pthread_mutex_destroy(&mutex);
	return 0;
}

// 임계영역 : 반복문을 포함해서 넓게 잡음
void* thread_inc(void* arg)
{
	int i;
	pthread_mutex_lock(&mutex);
	for (i = 0; i < 50000000; i++)
		num += 1;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

// 임계영역 : 반복문 안에 변수의 값을 바꾸는 코드로 좁게 잡음
void* thread_des(void* arg)
{
	int i;
	for (i = 0; i < 50000000; i++)
	{
		pthread_mutex_lock(&mutex);
		num -= 1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}