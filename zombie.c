#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	// fork�Լ� ����
	pid_t pid = fork();

	if (pid == 0)	// �ڽ� ���μ���
	{
		puts("Hi I'm a child process");
	}

	else			// �θ� ���μ��� 
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