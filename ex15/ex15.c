#include <stdio.h>
// T4&T5
void print_arguments(int count, char **pointers)
{
	for(int i = 0; i < count; i++)
	{
		printf("argv[%d] = %s, value address = %p\n", i, pointers[i], pointers + i);
	}
}


// T7以函数方法重新打印
void print_with_function(int method, int *ages, char **names, int count)
{
	printf("method == %d\n", method);

	for (int i = 0; i < count; i++) {
		if (method == 0) {
			printf("%s %d\n", names[i], ages[i]);
		} else if (method == 1) {
			printf("%s %d\n", *(names + i), *(ages + i));
		} else if (method == 2) {
			char **cur_name = names + i;
			int *cur_age = ages + i;
			printf("%s %d\n", *cur_name, *cur_age);
		}
	}
}

// T8 将T7中的for改为while
// while较为方便，指针只有数组的开始而无结束，遍历更方便
void print_with_whileFunction(int method, int *ages, char **names)
{
	printf("method == %d\n", method);
	int i = 0;
	while (ages[i] && names[i]) {
		if (method == 0) {
			printf("%s %d\n", names[i], ages[i]);
		} else if (method == 1) {
			printf("%s %d\n", *(names + i), *(ages + i));
		} else if (method == 2) {
			char **cur_name = names + i;
			int *cur_age = ages + i;
			printf("%s %d\n", *cur_name, *cur_age);
		}
		i++;
	}
}



int main(int argc, char *argv[])
{

	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {"ALAN", "FRANK", "MARY", "JOHN", "LISA"};
	int count = sizeof(ages) / sizeof(int);
	for (int i = 0; i < count; i++)
	{
		printf("%s %d\n", names[i], ages[i]);
	}
	printf("___________________________________\n");

	int *cur_age = ages;
	char **cur_name = names;

	for (int i = 0; i < count; i++)
	{
		printf("%s, %d. \n", cur_name[i], cur_age[i]);
	}

	printf("____________________________________\n");

	// T5将获取值与获取地址组合
	for (cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++)
	{
		printf("%s %d\n", *cur_name, *cur_age);
		printf("%p, %p\n", cur_name, cur_age);
	}

	for (char **p = argv; p - argv < argc; p++)
	{
		printf("%s\n", *p);
	}
	print_arguments(argc, argv);
	print_with_function(0, ages, names, count);
	print_with_function(1, ages, names, count);
	print_with_function(2, ages, names, count);
	print_with_whileFunction(0, ages, names);
	print_with_whileFunction(1, ages, names);
	print_with_whileFunction(2, ages, names);
	return 0;
}
