#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>

struct Person
{
	char *name;
	int age;
	int height;
	int weight;
};

struct Person *Person_create(char *name, int age, int height, int weight)
{
	struct Person *who = malloc(sizeof(struct Person));
	assert(who != NULL);
	who->name = strdup(name);
	who->age = age;
	who->height = height;
	who->weight = weight;
	return who;
}

void Person_destory(struct Person *who)
{
	assert(who != NULL);
	free(who->name);
	free(who);
}

void Person_print(struct Person *who)
{
	printf("Name: %s\n", who->name);
	printf("\tAge: %d\n", who->age);
	printf("\tHeight: %d\n", who->height);
	printf("\tWeight: %d\n", who->weight);
}

int main(int argc, char *argv[])
{
	struct Person *joe = Person_create(
	"Joe Alex",
	32,
	64,
	140);
	struct Person *frank = Person_create(
	"Frank",
	20,
	72,
	180);

	printf("joe %p\n", joe);
	printf("frank %p\n", frank);

	joe->age += 20;
	Person_print(frank);
	Person_print(joe);

//	Person_destory(joe);
//	Person_destory(frank);
//	Person_print(NULL);
	return 0;
}

