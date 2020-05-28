#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char* buf);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_sz;
	// fd_max : 가장 큰 파일 디스크립터 (+1 --> 0부터 시작하므로 몇개있는지 확인 가능)
	// fd_num : select 함수의 반환값을 저장
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	// 연결 요청을 받는 socket도 데이터 수신을 받는 것임
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;		// 3

	while (1)
	{
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;
		
		if (fd_num == 0)
			continue;

		for (i = 0; i < fd_max + 1; i++)
		{
			// 변화가 있었는지 확인
			if (FD_ISSET(i, &cpy_reads))
			{
				// 서버 소켓에 연결요청이 들어왔다면
				if (i == serv_sock)	
				{
					adr_sz = sizeof(clnt_adr);
					// 연결 요청을 수락하고 새로운 socket을 생성
					clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &adr_sz);
					
					// 새로운 socket을 관찰 대상으로 넣어줌
					FD_SET(clnt_sock, &reads);
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					printf("connected client: %d \n", clnt_sock);
				}

				// 변경된 파일 디스크립터가 있고 그것이 server가 아니라면 실행
				// client로부터 데이터가 수신되었다는 의미
				else	// read message!
				{
					str_len = read(i, buf, BUF_SIZE);
					if (str_len == 0)
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d \n", i);
					} 
					else
					{
						write(i, buf, str_len);		// echo!
					}
				}
			}
		}
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