#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

// thread의 main 함수로 호출될 함수들 (각각 입력, 출력을 담당)
void* send_msg(void* arg);
void* recv_msg(void* arg);

void error_handling(char* msg);

char name[NAME_SIZE] = "[DEFAULT]";		// 사용자 이름
char msg[BUF_SIZE];						// 메세지 버퍼

// thread를 사용해서 입출력을 분리하는 구조
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;

	// 입출력을 분리해서 담당할 thread들의 id가 담길 변수 선언
	pthread_t snd_thread, rcv_thread;
	void* thread_return;
	if (argc != 4) {
		printf("Usage : %s <IP> <port> <name> \n", argv[0]);
		exit(1);
	}

	sprintf(name, "[%s]", argv[3]);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connet() error!");

	// 연결된 socket의 파일 디스크립터를 인자로 넘겨줌
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(sock);
	return 0;
}

// 메세지를 보내는 역할
void* send_msg(void* arg)
{
	int sock = *((int*)arg);
	// 사용자 이름 + 메세지 내용
	char name_msg[NAME_SIZE + BUF_SIZE];
	while (1)
	{
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			close(sock);
			exit(0);
		}

		// name_mag = name + msg
		sprintf(name_msg, "%s %s", name, msg);
		write(sock, name_msg, strlen(name_msg));
	}
	return NULL;
}

// 메세지를 받는 역할
void* recv_msg(void* arg)
{
	int sock = *((int*)arg);
	// 전달받은 message를 저장할 변수
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;
	while (1)
	{
		str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
		if (str_len == -1)
			return (void*)-1;
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}
	return NULL;
}
void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}