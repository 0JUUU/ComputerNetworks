#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// 자식프로세스가 zombie가 되지 않고 정상적으로 종료됨

// 자식 process 종료 발생 시 부모 프로세스가 signal sig_child를 받고 실행시키는 함수
// 내부의 waitpid 함수를 blocking 상태가 되지 않도록 실행
// 종료된 자식 프로세스의 id와 반환값을 출력
void read_childproc(int sig)
{
	int status;
	// blocking 상태에 빠지지않는 옵션을 가짐
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

	// SIGCHLD에 대한 signal 등록
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
		// 첫번째 자식 프로세스
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
			// 두번째 자식 프로세스
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