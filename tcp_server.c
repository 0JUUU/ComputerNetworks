#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int serv_sock;		// ���� ����
	int clnt_sock;		// Ŭ���̾�Ʈ ���� : ���� ������ Ŭ���̾�Ʈ�� �ۼ��� �ϱ� ���� ����

	struct sockaddr_in serv_addr;	// ���� �ּ� ���� ������ ���� ����ü
	struct sockaddr_in clnt_addr;	// Ŭ���̾�Ʈ �ּ� ���� ������ ���� ����ü
	socklen_t clnt_addr_size;

	char message[] = "Hello World!";	// �۽��� ������

	if (argc != 2)		// ���� ���� ������ ������ ��, ��Ʈ ��ȣ�� ���� or �߸� �Ǿ��� �� ������ ���� ����
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);	// 1. ������ ����(��ȭ�� �޴� �뵵�� ���� ����)
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));		// ���� address�� �ʱ�ȭ

	// �ּ� ���� ����ü�� �ּ� ������ �����ϴ� ����
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));		// argv[1] = ��Ʈ��ȣ

	if (bind(serv_sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)	// IP�� PORT��ȣ�� �Ҵ�(�ּ� ���� �Ҵ�)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)		// �����û ���ɻ��·� ���� --> �� �Լ��� ȣ��Ǿ�� server ����(= listening ����)�̶�� �� �� ����
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);

	// accept : ���� ��û�� �� Ŭ���̾�Ʈ�� ����� �ϴ� ���ο� ������ ����
	clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_addr, &clnt_addr_size);	// �����û�� ���� ����
	if (clnt_sock == -1)
		error_handling("accept() error");

	// hello, world ������ Ŭ���̾�Ʈ�� �۽�
	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}