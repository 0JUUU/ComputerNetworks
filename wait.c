#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int status;
	pid_t pid = fork();

	// ù��° �ڽ�: 3�� return�� �ϸ� �����
	if (pid == 0)	
	{
		return 3;
	}

	else			 
	{
		printf("Child PID: %d \n", pid);
		// �ι�° �ڽ� ���μ��� ����
		pid = fork();
		if (pid == 0)
		{
			exit(7);
		}
		else
		{
			printf("Child PID: %d \n", pid);
			wait(&status);
			// ��������Ǿ��°�?
			if (WIFEXITED(status))
				printf("Child send one: %d \n", WEXITSTATUS(status));
			
			// �ٸ� �ڽ� ���μ����� �����ϸ� ������ �� 
			wait(&status);
			if (WIFEXITED(status))
				printf("Child send two: %d \n", WEXITSTATUS(status));
			sleep(30);
		}
	}
	return 0;
}