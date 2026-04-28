#include <stdio.h>
#include <ctype.h>
#include <string.h>

int can_print_it(char ch);
void print_letters(char arg[]);


void print_arguments(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++)
	{
		int length = strlen(argv[i]);
		print_letters(argv[i], length);
	}
}

// T2
void print_letters(char arg[], int length)
{
	for (int i = 0; i < length; i++)
	{
		char ch = arg[i];
		// T1 替代canPrintIt
		if (isalpha(ch) || isblank(ch))
		{
			if (islower(ch))
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


/* 
T3 
isalpha(ch)	是否是字母
isdigit(ch)	是否是数字
isalnum(ch)	是否是字母或数字
isblank(ch)	是否是空格或 tab
isspace(ch)	是否是空白字符，包括空格、\n、\t 等
ispunct(ch)	是否是标点符号
isupper(ch)	是否是大写字母
islower(ch)	是否是小写字母
isprint(ch)	是否是可打印字符
 */

// T4 收到
