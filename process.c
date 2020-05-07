#include <stdio.h>
#include <unistd.h>
int gval = 10;

int main(int argc, char* argv[])
{
	// ���μ��� id�� ������ ����
	pid_t pid;
	int lval = 20;
	gval++, lval += 5;		// 10 -> 11, 20 -> 25

	// �θ� ���μ������� fork �Լ��� ��ȯ���� �ڽ� ���μ����� id
	// �ڽ� ���μ��� : fork �Լ��� ��ȯ�� = 0

	// �� ������ ���μ��� 2�� ����(�θ�, �ڽ�)
	pid = fork();
	if (pid == 0)
		gval += 2, lval += 2;		// g = 9, l = 23
	else
		gval -= 2, lval -= 2;		// g = 13, l = 27

	if (pid == 0)
		printf("Child Proc: [%d, %d]\n", gval, lval);
	else
		printf("Parent Proc: [%d, %d]\n", gval, lval);
	return 0;
}