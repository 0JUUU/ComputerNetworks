#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void* handle_clnt(void* arg);		// thread의 main함수로 사용될 함수
void send_msg(char* msg, int len);	// msg 보낼 때 사용하는 함수
void error_handling(char* msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;		// mutex로 동기화를 관리하는 코드 -> mutex 선언

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// 자물쇠 생성
	pthread_mutex_init(&mutx, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)	
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)	
		error_handling("listen() error");

	// accept 함수로 연결요청을 수락 -> 새로운 connected 소켓을 만듦
	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;		// client 개수 후위연산으로 증가(임게영역)
		pthread_mutex_unlock(&mutx);

		// thread를 새롭게 열어서 해당 thread가 client와 데이터 송수신을 담당할 수 있도록 해줌
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);		
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}

void* handle_clnt(void* arg)
{
	// socket의 파일 디스크립터
	int clnt_sock = *((int*)arg);
	int str_len = 0, i;
	char msg[BUF_SIZE];

	// 받은 msg를 send_msg에 넣어줌
	while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
		send_msg(msg, str_len);

	// 자물쇠를 걺
	pthread_mutex_lock(&mutx);

	// client 소켓 배열에 있던 연결 종료된 소켓 파일 디스크립터를 정리 (임게영역)
	for (i = 0; i < clnt_cnt; i++)
	{
		if (clnt_sock == clnt_socks[i])
		{
			while (i < clnt_cnt - 1)
			{
				clnt_socks[i] = clnt_socks[i + 1];
				i++;
			}
			break;
		}
	}
	// 연결된 client 개수 (임계영역)
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

// send to all (접속된 모든 client들에게 메세지를 보내는 함수)
void send_msg(char* msg, int len)
{
	int i;
	pthread_mutex_lock(&mutx);
	// 임계영역
	for (i = 0; i < clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
