#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int i = argc;
	// T1
	while(i > 0) {
		printf("arg %d: %s\n", i, argv[i - 1]);
		i--;
	}

	char *states[] = {"California", "Oregon", "Washington", "Texas"};

	int num_states = sizeof(states) / sizeof(states[0]);
	i = 0;
	while(i < num_states) {
		printf("state %d: %s\n", i, states[i]);
		i++;
	}
	// T2&T3
	i = 0;
	while(i < num_states) {
		states[i] = argv[i];
		printf("state %d: %s\n", i, states[i]);
		i++;
	}
	// T4只是将字符串的地址传递给states，两个变量指向同一处	


	char str[] = "hello";
	char *p1 = str;
	char *p2 = p1;
	p1[0] = 'x';
	printf("str: %s\n", str);
	printf("p1: %s\n", p1);
	printf("p2: %s\n", p2);

	char origin[] = "test";
	char copy[] = "wuhu";
	strcpy(copy, origin);
	printf("origin: %s\n", origin);
	printf("copy: %s\n", copy);
	copy[0] = 'x';
	printf("origin: %s\n", origin);
	printf("copy: %s\n", copy);


	return 0;
}
