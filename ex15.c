#include <stdio.h>

int main(int argc, char *argv[])
{
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {"ALAN", "FRANK", "MARY", "JOHN", "LISA"};
	int count = sizeof(ages) / sizeof(int);
	for(int i = 0; i < count; i++)
	{
		printf("%s %d\n", names[i], ages[i]);
	}
	printf("___________________________________\n");

	int *cur_age = ages;
	char **cur_name = names;
	
	for(int i = 0; i < count; i++)
	{
		printf("%s, %d. \n", cur_name[i], cur_age[i]);
	}

	printf("____________________________________\n");

	for(cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++)
	{
		printf("%s %d\n", *cur_name, *cur_age);
	}
	return 0;
}
