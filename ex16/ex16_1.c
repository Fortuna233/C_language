#include<stdio.h>
#include<string.h>

struct Person
{
	char name[10];
	int age;
};

void Person_print(struct Person who)
{
	printf("Name: %s\n", who.name);
	printf("Age: %d\n", who.age);
}

int main()
{
	struct Person joe = {"Joe", 52};
	Person_print(joe);
	return 0;
}

