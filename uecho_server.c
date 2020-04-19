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
	// 파일 디스크립터를 저장할 데이터
	int serv_sock;
	char message[BUF_SIZE];
	int str_len;
	// 구조체 크기를 담을 변수
	socklen_t clnt_adr_sz;

	// 주소정보를 담을 구조체
	struct sockaddr_in serv_adr, clnt_adr;
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// SOCK_STREAM : TCP / SOCK_DGRAM : UDP --> 비연결지향 
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("UDP socket creation error");

	// sockaddr_in 타입(꼭 기억해두기!)
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// 서버 역할을 하기 때문에 주소 할당은 필수, UPD 는 서버랑 클라이언트를 구분하지 않지만, 
	// 목적지로 사용되기 때문에 주소 할당을 해야함
	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	// while문을 통해 여러개의 클라이언트와 계속 통신 가능
	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);

		// 다른 옵션을 사용하지 않기 때문에 3번째 인자에 0
		// 얼만큼 읽어왔는지 str_len이 알려줌
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, 
			(struct sockaddr*) & clnt_adr, &clnt_adr_sz);

		// 수신할 데이터를 그대로 보내주기 위하여 사용
		// recvfrom에서 받은 것을 clnt_adr로 보낸다.
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

