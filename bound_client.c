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
	char msg1[] = "Hi!";
	char msg2[] = "I'm another UDP host!";
	char msg3[] = "Nice to meet you";

	// 데이터가 수신되었을 때 어디서 왔는지 확인하는 구조체
	// 자신의 IP나 port번호가 담긴 구조체는 선언 X
	struct sockaddr_in your_adr;
	socklen_t your_adr_sz;
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&your_adr, 0, sizeof(your_adr));
	your_adr.sin_family = AF_INET;
	your_adr.sin_addr.s_addr = inet_addr(argv[1]);
	your_adr.sin_port = htons(atoi(argv[2]));

	// sendto를 통해 데이터 전달
	// TCP에서는 write를 3번 호출하여 데이터를 보내더라도 데이터의 경계가 없기 때문에 데이터가 잘 도착했다면 
	// 수신 측에서는 read함수 한번만 호출해도 모두 불러올 수 있었음
	// UDP에서는 데이터를 빠르게 다 보내더라도 이 경우 3개의 recvfrom을 호출해야함
	sendto(sock, msg1, sizeof(msg1), 0,
		(struct sockaddr*) & your_adr, sizeof(your_adr));
	sendto(sock, msg2, sizeof(msg2), 0,
		(struct sockaddr*) & your_adr, sizeof(your_adr));
	sendto(sock, msg3, sizeof(msg3), 0,
		(struct sockaddr*) & your_adr, sizeof(your_adr));

	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}