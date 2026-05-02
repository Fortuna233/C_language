#include<stdio.h>
#include<string.h>

struct Person
{
	char *name;
	int age;
};

void Person_print(struct Person who)
{
	printf("Name: %s\n", who.name);
	printf("Age: %d\n", who.age);
}

void Person_older(struct Person who)
{
	who.age += 20;
}

int main()
{
	//T1 不再依靠malloc在堆上申请内存，而是直接在栈上创建struct
	//T2 ->是指向结构体变量的指针访问该结构体所用表示，当变量名直接为结构体时，直接使用.访问. if it's a must to access with -> when using pointer, (*p)._

	//T3 如果不利用指针传递结构体，函数所得结构体为原结构体的拷贝
	struct Person joe = {"Joe", 52};
	Person_print(joe);
	joe.age += 300;
	Person_print(joe);
	Person_older(joe);
	printf("Name: %d\n", joe.age); 


	struct Person *wuhu = &joe;
	printf("Name: %s\n", (*wuhu).name); 
	printf("Age: %d\n", (*wuhu).age); 
	return 0;
}

