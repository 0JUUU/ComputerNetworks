#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void error_handling(char* message);

int main(int argc, char* argv[])
{
	// file descriptor ����
	int sock;
	char opmsg[BUF_SIZE];
	int result, opnd_cnt, i;

	// �ּ������� ������ ����ü(sockaddr ����ü���� ������ �˾Ƶα�)
	struct sockaddr_in serv_adr;
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// TCP ����
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	// ���� ���� �ּ������� �������
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected.............");

	fputs("Operand count: ", stdout);
	scanf("%d", &opnd_cnt);
	// 1����Ʈ ���·� �����ϱ�� ��� --> char�� ����ȯ
	opmsg[0] = (char)opnd_cnt;

	// ù��° : opmsg[1]�� ����, �ι�° : opmsg[5] --> opnd_cnt ���� 0��°�� ����Ǿ�����
	for (i = 0; i < opnd_cnt; i++)
	{
		printf("Operand %d: ", i + 1);

		// char�� �迭�� int������ �����ϱ� ���� �����͸� ���(�ּҿ� ���� �������ְ�����)
		scanf("%d", (int*)&opmsg[i * OPSZ + 1]);
	}
	// ���๮�ڳ� �ι��ڸ� ǥ���Է¹��ۿ��� ����(���� �Է¿� ���)
	fgetc(stdin);
	fputs("Operator: ", stdout);
	scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);

	// opmsg�� ������ ����
	write(sock, opmsg, opnd_cnt * OPSZ + 2);
	read(sock, &result, RLT_SIZE);

	printf("Operation result: %d \n", result);
	close(sock);

	return 0;
}
void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}