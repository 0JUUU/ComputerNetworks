#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void* read(void* arg);
void* accu(void* arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

// 임계영역으로의 동시 접근 동기화가 아닌 접근 순서의 동기화와 관련된 예제
int main(int argc, char* argv[])
{
	pthread_t id_t1, id_t2;
	sem_init(&sem_one, 0, 0);	// 초기값 : 0
	sem_init(&sem_two, 0, 1);	// 초기값 : 1

	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	return 0;
}

void* read(void* arg)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		fputs("Input num: ", stdout);

		sem_wait(&sem_two);		// 1 -> 0
		scanf("%d", &num);
		sem_post(&sem_one);		// 0 -> 1
	}
	return NULL;
}

void* accu(void* arg)
{
	// num을 가져와서 합산하는 함수
	int sum = 0, i;
	for (i = 0; i < 5; i++)
	{
		fputs("Input num: ", stdout);

		sem_wait(&sem_one);		// 0 ->(1로 증가시키기 전까지는 대기 상태에 놓임) 1 -> 0
		sum += num;
		sem_post(&sem_two);		// 1 ->(0으로 감소시키기 전에 대기 상태에 놓임) 0 -> 1
	}
	printf("Result: %d \n", sum);
	return NULL;
}