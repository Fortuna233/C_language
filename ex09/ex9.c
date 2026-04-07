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

	for(int i = 0; i < 10; i++)
	{
		printf("%d: %d\n", i, A[i]);
	}

	// T1&T2
	int ex_numbers[4] = {0};
	ex_numbers[0] = 'a';
	printf("ex_numbers[0]: %d\n", ex_numbers[0]);
	printf("size_of_int: %ld\n", sizeof(int));
	char ex_name[] = "WUH";
	ex_name[0] = 64;
	printf("ex_name[0]: %c\n", ex_name[0]);
	
	// T3
	printf("%c\n", name[0]);
	putchar(name[0]);
	printf("\n");
	fputc(name[0], stdout);
	printf("\n");

	// T4 Z:5A E:45 D:44 \0:00
	// -> 0044455A
	int whole = *(int *)name;
	printf("%d\n", whole);
	printf("%#x\n", whole);
	// T6
	// 指针指向只读常量区，无法更改
	/*
	char *ex_name = "WUH";
	ex_name[0] = 1;
	printf("ex_name[0]: %c", ex_name[0]);
	*/

	return 0;
}

