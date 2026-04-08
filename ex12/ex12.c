#include <stdio.h>

int main(int argc, char *argv[]) {

	/*
	if(argc == 1)
	{
		printf("one argument\n");
	// T4
	// printf("no argument\n");
	}
	else if(argc > 1 && argc < 4)
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
	*/
	// T3
	int i = 0;
	while(i < 10) {
		printf("argv[%d]: %s\n", i, argv[i]);
		i++;
		if (i == 5) {
			break;
		}
	}

	return 0;
	/* 
	&&：逻辑与（两边都为真，结果才真）
	||：逻辑或（一边为真，结果就真）
	!：逻辑非（真变假，假变真）
	关系运算符（比较大小）
	== 等于、!= 不等于、< 小于、> 大于、<= 小于等于、>= 大于等于
	*/
}


