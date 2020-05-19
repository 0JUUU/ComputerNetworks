#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// �ڽ����μ����� zombie�� ���� �ʰ� ���������� �����

// �ڽ� process ���� �߻� �� �θ� ���μ����� signal sig_child�� �ް� �����Ű�� �Լ�
// ������ waitpid �Լ��� blocking ���°� ���� �ʵ��� ����
// ����� �ڽ� ���μ����� id�� ��ȯ���� ���
void read_childproc(int sig)
{
	int status;
	// blocking ���¿� �������ʴ� �ɼ��� ����
	pid_t id = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status))
	{
		printf("Removed proc id: %d \n", id);
		printf("Child send: %d \n", WEXITSTATUS(status));
	}
}

int main(int argc, char* argv[])
{
	pid_t pid;
	struct sigaction act;
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	// SIGCHLD�� ���� signal ���
	sigaction(SIGCHLD, &act, 0);

	pid = fork();
	if (pid == 0)
	{
		puts("Hi! I'm child process");
		sleep(10);
		return 12;
	}
	else
	{
		// ù��° �ڽ� ���μ���
		printf("Child proc id: %d \n", pid);
		pid = fork();
		if (pid == 0)
		{
			puts("Hi! I'm child process");
			sleep(10);
			exit(24);
		}
		else
		{
			int i;
			// �ι�° �ڽ� ���μ���
			printf("Child proc id: %d \n", pid);
			for (i = 0; i < 5; i++)
			{
				puts("wait...");
				sleep(5);
			}
		}
		return 0;
	}

}