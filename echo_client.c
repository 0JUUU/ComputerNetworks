#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	int recv_len, recv_cnt;
	struct sockaddr_in serv_adr;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	// IP
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	// PORT
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

	while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		// 뒤에서 받은 크기와 비교하기 위해 write값을 저장
		str_len = write(sock, message, strlen(message));

		recv_len = 0;
		// 여러번 read함수를 호출할 수 있도록
		while (recv_len < str_len)				// != 하지 않는 이유 : error 때문에 recv_len이 str_len보다 커질 수 있어 무한루프 가능성이 존재
		{
			// recv_len : 이전 데이터 이후부터 데이터를 작성할 수 있도록
			recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
			if (recv_cnt == -1)
				error_handling("read() error!");
			// 전체 데이터 길이를 받아올 수 있음
			recv_len += recv_cnt;
		}

		message[recv_len] = 0;
		printf("Message from server: %s", message);
	}

	close(sock);
	return 0;

}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}