#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int serv_sock;		// 서버 소켓
	int clnt_sock;		// 클라이언트 소켓 : 서버 측에서 클라이언트와 송수신 하기 위한 소켓

	struct sockaddr_in serv_addr;	// 서버 주소 정보 저장을 위한 구조체
	struct sockaddr_in clnt_addr;	// 클라이언트 주소 정보 저장을 위한 구조체
	socklen_t clnt_addr_size;

	char message[] = "Hello World!";	// 송신할 데이터

	if (argc != 2)		// 서버 실행 파일을 실행할 때, 포트 번호가 누락 or 잘못 되었을 떄 나오는 에러 문구
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);	// 1. 소켓의 생성(전화를 받는 용도의 소켓 생성)
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));		// 서버 address를 초기화

	// 주소 정보 구조체에 주소 정보를 저장하는 과정
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));		// argv[1] = 포트번호

	if (bind(serv_sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)	// IP와 PORT번호의 할당(주소 정보 할당)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)		// 연결요청 가능상태로 변경 --> 이 함수가 호출되어야 server 소켓(= listening 소켓)이라고 할 수 있음
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);

	// accept : 연결 요청을 한 클라이언트와 통신을 하는 새로운 소켓을 생성
	clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_addr, &clnt_addr_size);	// 연결요청에 대한 수락
	if (clnt_sock == -1)
		error_handling("accept() error");

	// hello, world 문구를 클라이언트로 송신
	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}