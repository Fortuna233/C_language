#include <stdio.h>

int main(int argc, char *argv[]) {

	if(argc == 2)
	{
		printf("one argument\n");
	}
	else if(argc > 2 && argc < 5)
	{
		printf("arguments:\n");

		for(int i = 0; i < argc; i++)
		{
			printf("%s ", argv[i]);
		}
		printf("\n");
	}
	else
	{
		printf("too many arguments\n");
	}

	return 0;
}


