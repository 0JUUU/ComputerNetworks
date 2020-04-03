// 연결을 요청하는 소켓의 구현
// '소켓의 생성'과 '연결의 요청'으로 구분
// 리스닝 소켓과 달리 구현의 과정이 매우 간단
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;		// 주소 정보 저장
	char message[30];					// 수신할 메세지를 담음
	int str_len;
	int read_len;
	int idx;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);			// 소켓 생성(연결 요청 및 데이터 송,수신을 위해 사용)
	if (sock == -1)
		error_handling("socket() error");

	// 구조체 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);		// argv[1] : 클라이언트 실행 파일을 실행할 때 입력한 127.0.0.1의 IP 주소
	serv_addr.sin_port = htons(atoi(argv[2]));			// argv[2] : 9190 PORT 번호
														// 이 주소들은 서버로 연결 요청을 보낼 때 사용


	// connect 함수 --> 클라이언트 소켓이 연결 요청을 함(listen 함수를 통해 연결 요청 가능한 서버 소켓으로 전송, 서버 소켓에서 accept함수를 통해 연결 요청 수락 --> 데이터 송수신이 가능)
	if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connet() error!");

	while (read_len = read(sock, &message[idx++], 1))
	{
		if (str_len == -1)
		{
			error_handling("read() error!");
			break;
		}
		str_len += read_len;
	}
	// 소켓으로부터 읽어들어오는 함수

	printf("Message from server : %s \n", message);
	printf("Function read call count: %d \n", str_len);
	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}