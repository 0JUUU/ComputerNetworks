#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[])
{
	// ���� ��ũ���͸� ������ ������
	int serv_sock;
	char message[BUF_SIZE];
	int str_len;
	// ����ü ũ�⸦ ���� ����
	socklen_t clnt_adr_sz;

	// �ּ������� ���� ����ü
	struct sockaddr_in serv_adr, clnt_adr;
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// SOCK_STREAM : TCP / SOCK_DGRAM : UDP --> �񿬰����� 
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("UDP socket creation error");

	// sockaddr_in Ÿ��(�� ����صα�!)
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// ���� ������ �ϱ� ������ �ּ� �Ҵ��� �ʼ�, UPD �� ������ Ŭ���̾�Ʈ�� �������� ������, 
	// �������� ���Ǳ� ������ �ּ� �Ҵ��� �ؾ���
	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	// while���� ���� �������� Ŭ���̾�Ʈ�� ��� ��� ����
	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);

		// �ٸ� �ɼ��� ������� �ʱ� ������ 3��° ���ڿ� 0
		// ��ŭ �о�Դ��� str_len�� �˷���
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, 
			(struct sockaddr*) & clnt_adr, &clnt_adr_sz);

		// ������ �����͸� �״�� �����ֱ� ���Ͽ� ���
		// recvfrom���� ���� ���� clnt_adr�� ������.
		sendto(serv_sock, message, str_len, 0,
			(struct sockaddr*) & clnt_adr, clnt_adr_sz);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

