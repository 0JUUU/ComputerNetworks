#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


// 서버에서 먼저 종료할 경우 같은 port번호로 한동안 접속 불가
#define TRUE 1
#define FALSE 0
void error_handling(char* message);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	char message[30];
	int option, str_len;
	socklen_t optlen, clnt_adr_sz;
	struct sockaddr_in serv_adr, clnt_adr;

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);	// TCP
	if (serv_sock == -1)
		error_handling("socket() error");

	/*
	// 바로 사용할 수 있도록 도와줌 --> 주소를 재할당 가능하도록 함 
	optlen = sizeof(option);
	option = TRUE;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
	*/

	optlen = sizeof(option);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// 주소 할당을 하기 위해 bind함수 사용
	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)))
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen error");
	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);

	while ((str_len = read(clnt_sock, message, sizeof(message))) != 0)
	{
		write(clnt_sock, message, str_len);
		// 표준 출력을 의미하는 1
		write(1, message, str_len);
	}
	close(clnt_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}