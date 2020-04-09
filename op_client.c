#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void error_handling(char* message);

int main(int argc, char* argv[])
{
	// file descriptor 저장
	int sock;
	char opmsg[BUF_SIZE];
	int result, opnd_cnt, i;

	// 주소정보를 저장할 구조체(sockaddr 구조체와의 차이점 알아두기)
	struct sockaddr_in serv_adr;
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// TCP 소켓
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	// 서버 측의 주소정보를 담고있음
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected.............");

	fputs("Operand count: ", stdout);
	scanf("%d", &opnd_cnt);
	// 1바이트 형태로 저장하기로 약속 --> char로 형변환
	opmsg[0] = (char)opnd_cnt;

	// 첫번째 : opmsg[1]에 저장, 두번째 : opmsg[5] --> opnd_cnt 값이 0번째에 저장되어있음
	for (i = 0; i < opnd_cnt; i++)
	{
		printf("Operand %d: ", i + 1);

		// char형 배열에 int형으로 저장하기 위해 포인터를 사용(주소에 직접 저장해주고있음)
		scanf("%d", (int*)&opmsg[i * OPSZ + 1]);
	}
	// 개행문자나 널문자를 표준입력버퍼에서 지움(다음 입력에 대비)
	fgetc(stdin);
	fputs("Operator: ", stdout);
	scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);

	// opmsg를 서버로 전송
	write(sock, opmsg, opnd_cnt * OPSZ + 2);
	read(sock, &result, RLT_SIZE);

	printf("Operation result: %d \n", result);
	close(sock);

	return 0;
}
void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}