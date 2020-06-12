#include <stdio.h>
#include <pthread.h>

void* thread_summation(void* arg);		// thread의 main 함수

int sum = 0;		// 두 개의 thread가 접근 가능한 변수

int main(int argc, char* argv[])
{
	pthread_t id_t1, id_t2;

	// 덧셈의 범위, thread main 함수의 인자로 사용
	int range1[] = { 1, 5 };	
	int range2[] = { 6, 10 };
	
	pthread_create(&id_t1, NULL, thread_summation, (void*)range1);
	pthread_create(&id_t2, NULL, thread_summation, (void*)range2);
	
	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);
	printf("result: %d \n", sum);
	return 0;
}

void* thread_summation(void* arg)
{
	int start = ((int*)arg)[0];
	int end = ((int*)arg)[1];

	while (start <= end)
	{
		sum += start;
		start++;
	}
	return NULL;
}