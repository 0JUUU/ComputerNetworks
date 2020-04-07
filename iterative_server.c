#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024			// 선행처리기
void error_handling(char* message);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;		// serv_sock : 연결 요청을 받는 소켓 , clnt_sock : accpet 이후에 새로 만들어지는 데이터 송수신을 담당하는 소켓 
	char message[BUF_SIZE];		

	// read 함수에서 몇글자 읽어왔는지 알려주는 변수 : str_len
	int str_len, i;				

	struct sockaddr_in serv_adr;		// 서버 소켓 주소 할당할 구조체
	struct sockaddr_in clnt_adr;		// accept 인자로 들어갈 구조체, 연결 요청을 한 클라이언트 주소 담음
	socklen_t clnt_adr_sz;				// clnt_adr 값의 크기를 담음

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// TCP 소켓 생성(시험에 내기 좋은 부분)
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	if (serv_sock == -1)
	{
		error_handling("socket() error");
	}

	// 주소 정보 담는 구조체를 초기화
	memset(&serv_adr, 0, sizeof(serv_adr));

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	// 연결 요청 대기 큐의 개수 : 5 --> 연결 요청을 가능한 상태로 만들어줌
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);

	for (i = 0; i < 5; i++)
	{
		// 연결 요청을 수락을 하겠다!
		clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		// 데이터 송수신을 담당하는 소켓 --> 읽어와서 message 배열에 저장(에코 서버)
		while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
			write(clnt_sock, message, str_len);

		// 클라이언트 소켓만 먼저 닫음
		close(clnt_sock);
	}

	// 연결 요청 대기큐가 비어있을 때 서버 소켓을 닫음
	close(serv_sock);
	return 0;
}


void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}