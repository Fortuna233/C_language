#include <stdio.h>

int main(int argc, char *argv[])
{
	int distance = 100;
	float power = 2.345f;
	double super_power = 56789.4532;
	//浮点数与双精度数都用%f占位符
	char initial = 'A';   
	char first_name[] = "Zed";
	char last_name[] = "Shaw";
	printf("You are %X,%o miles away.\n", distance, distance);
    printf("You have %.2f levels of power.\n", power);
    printf("You have %f awesome super powers.\n", super_power);
    printf("I have an initial %c.\n", initial);
    printf("I have a first name %s.\n", first_name);
    printf("I have a last name %s.\n", last_name);
    printf("My whole name is %c %s. %s.\n", initial, first_name, last_name);
		printf("\n");
		return 0;
}
