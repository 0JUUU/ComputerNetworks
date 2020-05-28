#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

// select : window������ ��� ���� -> �̽ļ� good
//			�������� ������ �� ���� ��Ƴ��� �ѹ��� ����, ���� ����
int main(int argc, char* argv[])
{
	// reads : ������ ���� ���ο� ������ ��, 
	// temps : select �Լ��� ���ڷ� reads�� �ѱ�� �Ǹ� select �Լ� ȣ�� ���� 
	// ��ϵ� ���� ��ũ���� ����Ʈ �� ��ȭ�� �ִ� �͸� 1�� ����� �������� 0���� ��ȯ --> �ٽ� ��������� �����ϱ� ���ؼ��� ����� �ʿ���
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	// ��� bit 0���� �ʱ�ȭ
	FD_ZERO(&reads);
	FD_SET(0, &reads);	// 0 is standard input(console)

	//timeout.tv_sec = 5;
	//timeout.tv_usec = 5000;

	while (1)
	{
		temps = reads;

		// while �ۿ��� ���������� select �Լ� ���� �ʱ�ȭ �����༭
		// ���߿��� ��� timeout�� �߻�
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		// �����ϰ� ���� ��� 1��, ������ �Է¿��� ������ �־ readset�ڸ��� temps�� ����
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
			// � ���� ��ũ���Ͱ� ����Ǿ����� Ȯ��
			// ǥ���Է� ������ ���� ��ũ�����̹Ƿ� ù���� ���ڿ� 0�� �־���
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