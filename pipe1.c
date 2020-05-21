#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	// pipe�� �Ա�, �ⱸ �����ϹǷ� 2��
	int fds[2];
	char str[] = "Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	// pipe ��ü�� ����Ǵ� ���� �ƴ� ���� �����ϵ���
	// file descriptor�� ����(�θ� ���μ����� ������- �޸� ������ �ü���� �������)
	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		// fds[1] : pipe�� �Ա�
		write(fds[1], str, sizeof(str));
	}
	else
	{
		// fds[0] : pipe�� �ⱸ
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}
	return 0;
}