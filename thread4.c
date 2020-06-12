#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100		// 만들 thread의 개수

void* thread_inc(void* arg);
void* thread_des(void* arg);
long long num = 0;

// 여러 thread가 프로세스로부터 받은 작업을 각각 진행하고 그 결과를 프로세스가 출력 -> 워커 프로세스
// 백 개의 thread가 50개씩 나누어져 thread_inc, thread_des를 각각 진행해서 1을 증가 or 감소
// 예상 결과값 : 0
// 실제 결과값 : 0이 아닌 다른 값이 나오게 됨
int main(int argc, char* argv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	printf("sizeof long long: %d \n", sizeof(long long));		// 8 byte (64 bit)
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
	return 0;
}

void* thread_inc(void* arg)
{
	int i;
	for (i = 0; i < 50000000; i++)
		num += 1;
	return NULL;
}

void* thread_des(void* arg)
{
	int i;
	for (i = 0; i < 50000000; i++)
		num -= 1;
	return NULL;
}
