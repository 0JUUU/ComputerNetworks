#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 50
void error_handling(char* message);

// 파일을 서버로부터 전송받고 "Thank you" 라는 문자열을
// 서버에게 보내는 코드
int main(int argc, char* argv[])
{
	int sd;
	FILE* fp;

	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	fp = fopen("receive.dat", "wb");
	sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

	// 서버에 연결 요청을 보냄
	// client 소켓에 IP와 port번호가 할당되어 있지 않은데 connect 함수를 호출하면서 자동으로 붙게 됨
	connect(sd, (struct sockaddr*) & serv_adr, sizeof(serv_adr));

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
		fwrite((void*)buf, 1, read_cnt, fp);

	puts("Received file data");
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd); 
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}