#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	// fork함수 생성
	pid_t pid = fork();

	if (pid == 0)	// 자식 프로세스
	{
		puts("Hi I'm a child process");
	}

	else			// 부모 프로세스 
	{
		printf("Child Process ID: %d \n", pid);	
		sleep(30);
	}

	if (pid == 0)
		puts("End child process");
	else
		puts("End parent process");
	return 0;
}