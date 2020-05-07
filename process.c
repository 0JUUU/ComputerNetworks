#include <stdio.h>
#include <unistd.h>
int gval = 10;

int main(int argc, char* argv[])
{
	// 프로세스 id를 저장할 변수
	pid_t pid;
	int lval = 20;
	gval++, lval += 5;		// 10 -> 11, 20 -> 25

	// 부모 프로세스에서 fork 함수의 반환값은 자식 프로세스의 id
	// 자식 프로세스 : fork 함수의 반환값 = 0

	// 이 때부터 프로세스 2개 존재(부모, 자식)
	pid = fork();
	if (pid == 0)
		gval += 2, lval += 2;		// g = 9, l = 23
	else
		gval -= 2, lval -= 2;		// g = 13, l = 27

	if (pid == 0)
		printf("Child Proc: [%d, %d]\n", gval, lval);
	else
		printf("Parent Proc: [%d, %d]\n", gval, lval);
	return 0;
}