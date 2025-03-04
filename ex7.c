#include <stdio.h>

int main(int argc, char *arhv[])
{
	int bugs = 100;
	double bug_rate = 1.2;

	printf("You have %d bugs at the imaginary rate of %f.\n", bugs, bug_rate);

	unsigned long universe_of_defects = 1024L * 1024L * 1024L * 1024;
	printf("%ld\n", universe_of_defects);

	double expected_bugs = bugs * bug_rate;
	printf("%e\n", expected_bugs);

	char nul_byte = '\0';
	int care_percentage = bugs * nul_byte;
	printf("%d%%.\n", care_percentage);

	return 0;
}
