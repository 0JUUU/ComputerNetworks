#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024			// ����ó����
void error_handling(char* message);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;		// serv_sock : ���� ��û�� �޴� ���� , clnt_sock : accpet ���Ŀ� ���� ��������� ������ �ۼ����� ����ϴ� ���� 
	char message[BUF_SIZE];		

	// read �Լ����� ����� �о�Դ��� �˷��ִ� ���� : str_len
	int str_len, i;				

	struct sockaddr_in serv_adr;		// ���� ���� �ּ� �Ҵ��� ����ü
	struct sockaddr_in clnt_adr;		// accept ���ڷ� �� ����ü, ���� ��û�� �� Ŭ���̾�Ʈ �ּ� ����
	socklen_t clnt_adr_sz;				// clnt_adr ���� ũ�⸦ ����

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// TCP ���� ����(���迡 ���� ���� �κ�)
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	if (serv_sock == -1)
	{
		error_handling("socket() error");
	}

	// �ּ� ���� ��� ����ü�� �ʱ�ȭ
	memset(&serv_adr, 0, sizeof(serv_adr));

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	// ���� ��û ��� ť�� ���� : 5 --> ���� ��û�� ������ ���·� �������
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);

	for (i = 0; i < 5; i++)
	{
		// ���� ��û�� ������ �ϰڴ�!
		clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		// ������ �ۼ����� ����ϴ� ���� --> �о�ͼ� message �迭�� ����(���� ����)
		while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
			write(clnt_sock, message, str_len);

		// Ŭ���̾�Ʈ ���ϸ� ���� ����
		close(clnt_sock);
	}

	// ���� ��û ���ť�� ������� �� ���� ������ ����
	close(serv_sock);
	return 0;
}


void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}