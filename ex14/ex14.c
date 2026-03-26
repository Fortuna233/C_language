#include <stdio.h>
#include <ctype.h>
#include <string.h>

int can_print_it(char ch);
void print_letters(char arg[]);

void print_arguments(int argc, char *argv[])
{
	for(int i = 1; i < argc; i++)
	{
		print_letters(argv[i]);
	}
}


void print_letters(char arg[])
{
	for(int i = 0; i <= strlen(arg) / 2 ; i++)
	{
		char ch = arg[i];
		if(isalpha(ch) || isblank(ch))
		{
			if(islower(ch))
			{
				printf("'%c' == %d ", ch, ch);
				printf("lower");
			}
		}
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	print_arguments(argc, argv);
	return 0;
}
