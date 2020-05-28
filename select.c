#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

// select : window에서도 사용 가능 -> 이식성 good
//			여러개의 소켓을 한 곳에 모아놓고 한번에 관찰, 관리 가능
int main(int argc, char* argv[])
{
	// reads : 데이터 수신 여부에 관심을 둠, 
	// temps : select 함수에 인자로 reads를 넘기게 되면 select 함수 호출 이후 
	// 등록된 파일 디스크립터 리스트 중 변화가 있는 것만 1로 남기고 나머지는 0으로 변환 --> 다시 관찰대상을 관찰하기 위해서는 등록이 필요함
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	// 모든 bit 0으로 초기화
	FD_ZERO(&reads);
	FD_SET(0, &reads);	// 0 is standard input(console)

	//timeout.tv_sec = 5;
	//timeout.tv_usec = 5000;

	while (1)
	{
		temps = reads;

		// while 밖에서 설정했으면 select 함수 전에 초기화 못해줘서
		// 나중에는 계속 timeout이 발생
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		// 관찰하고 싶은 대상 1개, 데이터 입력에만 관심이 있어서 readset자리에 temps를 넣음
		result = select(1, &temps, 0, 0, &timeout);
		if (result == -1)
		{
			puts("select() error!");
			break;
		}
		else if (result == 0)
		{
			puts("Time-out!");
		}
		else 
		{
			// 어떤 파일 디스크립터가 변경되었는지 확인
			// 표준입력 관련한 파일 디스크립터이므로 첫번쨰 인자에 0을 넣어줌
			if (FD_ISSET(0, &temps))
			{
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console: %s", buf);
			}
		}
	}
	return 0;
}