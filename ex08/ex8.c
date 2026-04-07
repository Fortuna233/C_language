#include <stdio.h>


char full_name[] = {'Z', 'e', 'd', ' ', 'A', '.', ' ', 'S', 'h', 'a', 'w'};

int main(int agrc, char *argv[])
{
	int areas[] = {10, 12, 13, 14, 20};
	// T1
	areas[0] = 100;
	areas[1] = 10;
	char name[] = "Zed";
	// T2
	name[0] = 'w';
	// T3
	name[1] = areas[1] + 32;

	printf("%ld\n", sizeof(int));
	printf("%ld\n", sizeof(areas) / sizeof(int));
	printf("%d, %d\n", areas[0], areas[1]);
	printf("%d, %d\n", areas[10], areas[1]);
	printf("The size of a char: %ld\n", sizeof(char));
	printf("The size of name (char[]): %ld\n", sizeof(name));
	printf("The number of chars: %ld\n", sizeof(name) / sizeof(char));

	printf("The size of full_name (char[]): %ld\n", sizeof(full_name));
	printf("The number of chars: %ld\n", sizeof(full_name) / sizeof(char));

	printf("name=\"%s\" and full_name=\"%s\"\n", name, full_name);
	 return 0;
}
