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
	int sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t adr_sz;

	struct sockaddr_in serv_adr, from_adr;
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	// sock에 serv_adr(목적지 주소 정보)를 등록
	// 연결 요청을 보내는 것이 아니라 목적지 주소 정보를 등록하기 위해 connect 사용(TCP와의 차이점)
	connect(sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr));
	while (1)
	{
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);

		// application protocol
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		// connect를 하지 않았다면 목적지 주소 정보를 계속 등록해야함(1단계, 3단계 등록을 안해도됨 - 절약 가능)
		/*	
		sendto(sock, message, strlen(message), 0,
			(struct sockaddr*) & serv_adr, sizeof(serv_adr));
		*/
		write(sock, message, strlen(message));

		// connect를 사용하였기 때문에 발신지 정보를 담은 구조체를 넘겨주지 않아도됨
		/*
		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0,
			(struct sockaddr*) & from_adr, &adr_sz);
		*/
		str_len = read(sock, message, sizeof(message) - 1);

		message[str_len] = 0;
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