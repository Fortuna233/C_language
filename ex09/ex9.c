#include <stdio.h>

int main(int agrc, char *argv[])
{
	int numbers[4] = {0};
	char *name = "ZED";

	printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2], numbers[3]);

	printf("names: %c %c %c %c\n", name[0], name[1], name[2], name[3]);


	printf("names %s\n", name);
	numbers[0] = 'i';
	numbers[1] = 'q';
	numbers[2] = 3;
	numbers[3] = 4;

	printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2], numbers[3]);

	printf("names: %c %c %c %c\n", name[0], name[1], name[2], name[3]);

	printf("names %s\n", name);

	char *another = "Zed";
	printf("another: %s\n", another);

	printf("another: %c %c %c %c\n", another[0], another[1], another[2], another[3]);
	
	int A[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

	printf("%d\n", A[0]);
	for(int i = 0; i < 10; i++)
	{
		printf("%d: %d\n", i, A[i]);
	}

	return 0;
}

