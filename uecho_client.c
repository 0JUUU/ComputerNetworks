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

	// TCP와는 다르게 from_adr 존재
	struct sockaddr_in serv_adr, from_adr;
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// UDP 소켓 생성 (TCP : SOCK_STREAM)
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	
	// TCP라면 서버 주소 정보를 담는 구조체 초기화, connect 함수 호출 -> 서버와 연결을 설정
	// UDP : 연결 설정하지 않아도 됨 --> connect 함수 호출 없이 바로 데이터 입력
	while (1)
	{
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);

		// application protocol
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		// 이 시점부터 서버와 통신 시작
		// 이전 연결을 설정 X, 바로 sendto 함수부터 : TCP와의 차이점
		// &serv_adr을 통해 while문에서 sendto 함수가 실행될 때 어디로 가야하는지 알려줌
		sendto(sock, message, strlen(message), 0,
			(struct sockaddr*) & serv_adr, sizeof(serv_adr));
		adr_sz = sizeof(from_adr);

		// server로부터 echo된 데이터가 오기를 기다림
		// 발신지 정보를 from_adr에 넣음
		str_len = recvfrom(sock, message, BUF_SIZE, 0, 
			(struct sockaddr*) &from_adr, &adr_sz);
		
		// 문자열이 끝났음을 알려줌
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