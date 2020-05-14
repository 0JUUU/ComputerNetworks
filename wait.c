#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int status;
	pid_t pid = fork();

	// 첫번째 자식: 3을 return을 하며 종료됨
	if (pid == 0)	
	{
		return 3;
	}

	else			 
	{
		printf("Child PID: %d \n", pid);
		// 두번째 자식 프로세스 생성
		pid = fork();
		if (pid == 0)
		{
			exit(7);
		}
		else
		{
			printf("Child PID: %d \n", pid);
			wait(&status);
			// 정상종료되었는가?
			if (WIFEXITED(status))
				printf("Child send one: %d \n", WEXITSTATUS(status));
			
			// 다른 자식 프로세스가 종료하며 전달한 값 
			wait(&status);
			if (WIFEXITED(status))
				printf("Child send two: %d \n", WEXITSTATUS(status));
			sleep(30);
		}
	}
	return 0;
}