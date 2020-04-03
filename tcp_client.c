// ������ ��û�ϴ� ������ ����
// '������ ����'�� '������ ��û'���� ����
// ������ ���ϰ� �޸� ������ ������ �ſ� ����
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;		// �ּ� ���� ����
	char message[30];					// ������ �޼����� ����
	int str_len;
	int read_len;
	int idx;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);			// ���� ����(���� ��û �� ������ ��,������ ���� ���)
	if (sock == -1)
		error_handling("socket() error");

	// ����ü �ʱ�ȭ
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);		// argv[1] : Ŭ���̾�Ʈ ���� ������ ������ �� �Է��� 127.0.0.1�� IP �ּ�
	serv_addr.sin_port = htons(atoi(argv[2]));			// argv[2] : 9190 PORT ��ȣ
														// �� �ּҵ��� ������ ���� ��û�� ���� �� ���


	// connect �Լ� --> Ŭ���̾�Ʈ ������ ���� ��û�� ��(listen �Լ��� ���� ���� ��û ������ ���� �������� ����, ���� ���Ͽ��� accept�Լ��� ���� ���� ��û ���� --> ������ �ۼ����� ����)
	if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connet() error!");

	while (read_len = read(sock, &message[idx++], 1))
	{
		if (str_len == -1)
		{
			error_handling("read() error!");
			break;
		}
		str_len += read_len;
	}
	// �������κ��� �о������ �Լ�

	printf("Message from server : %s \n", message);
	printf("Function read call count: %d \n", str_len);
	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}