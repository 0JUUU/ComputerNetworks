#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	int fds[2];
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		write(fds[1], str1, sizeof(str1));
		// sleep(2)를 주석처리 한다면 종료되지 못하고 blocking 상태로 남아있음
		// 하나의 파이프로 양방향 통신을 할 때 문제점 발생
		// parent의 출력 내용이 child로 나오게 됨
		sleep(2);
		read(fds[0], buf, BUF_SIZE);
		printf("Child proc output: %s \n", buf);
	}
	else
	{
		read(fds[0], buf, BUF_SIZE);
		printf("Parent proc output: %s \n", buf);
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}
	return 0;
}