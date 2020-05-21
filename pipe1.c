#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	// pipe의 입구, 출구 존재하므로 2개
	int fds[2];
	char str[] = "Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	// pipe 자체가 복사되는 것이 아닌 접근 가능하도록
	// file descriptor를 복사(부모 프로세스에 생성됨- 메모리 공간은 운영체제가 만들어줌)
	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		// fds[1] : pipe의 입구
		write(fds[1], str, sizeof(str));
	}
	else
	{
		// fds[0] : pipe의 출구
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}
	return 0;
}