#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char* buf);

// 입출력 내용을 분할함
// 데이터 수신을 기다리는 것이 없어서(즉, 바로바로 출력돼서) 입력 안내문이 없음
int main(int argc, char* argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];

	// 서버의 주소 정보 담길 구조체
	struct sockaddr_in serv_adr;
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);			// TCP
	// 서버 주소 정보 구조체 초기화 --> 처음을 0으로 모두 초기화 하는 이유
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	pid = fork();
	if (pid == 0)
		write_routine(sock, buf);
	else
		read_routine(sock, buf);

	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
	while (1)
	{
		int str_len = read(sock, buf, BUF_SIZE);
		if (str_len == 0)
			return;
		buf[str_len] = 0;
		printf("Message from server: %s", buf);
	}
}

void write_routine(int sock, char* buf)
{
	while (1)
	{
		fgets(buf, BUF_SIZE, stdin);

		// TCP half close : 출력만 닫혀있고 입력은 열려있음
		if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
		{
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	}
}
void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}