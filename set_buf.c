#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char* message);

// ����� ������ ũ�⸦ �����ϰ� �ٽ� Ȯ��
int main(int argc, char* argv[])
{
	int sock;
	// ������ ����� ������ ���� ����� ���ÿ� ����
	int snd_buf = 1024 * 3, rcv_buf = 1024 * 3;			// 3072
	int state;
	socklen_t len;

	sock = socket(PF_INET, SOCK_STREAM, 0);

	// �Է��Լ��� ���� ũ�⸦ ����
	state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
	if (state)
		error_handling("setsocket() error!");

	// ����Լ��� ���� ũ�⸦ ����
	state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
	if (state)
		error_handling("setsocket() error!");

	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
	if (state)
		error_handling("getsocket() error!");

	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
	if (state)
		error_handling("getsocket() error!");

	printf("Input buffer size: %d \n", rcv_buf);
	printf("Output buffer size: %d \n", snd_buf);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}