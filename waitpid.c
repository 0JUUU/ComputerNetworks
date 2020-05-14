#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int status;
	pid_t pid = fork();

	if (pid == 0)
	{
		sleep(15);
		return 24;
	}

	else
	{
		// waitpid : 블로킹 상태에 놓이지 않게끔 해줌
		while (!waitpid(-1, &status, WNOHANG))
		{
			sleep(3);
			puts("sleep 3sec.");
		}

		if (WIFEXITED(status))
			printf("Child send one: %d \n", WEXITSTATUS(status));

	}
	return 0;
}