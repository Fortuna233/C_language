#include <stdio.h>
#include <ctype.h>
#include "dbg.h"


int print_a_message(const char *msg)
{
    printf("A STRING: %s\n", msg);

    return 0;
}


int uppercase(const char *msg)
{
    int i = 0;
	int max = 512;
    // BUG: \0 termination problems
    for(i = 0; msg[i] != '\0' && i < max; i++) {
        printf("%c", toupper(msg[i]));
    }

    printf("\n");

    return 0;
}

int lowercase(const char *msg)
{
    int i = 0;

	int max = 512;
    // BUG: \0 termination problems
    for(i = 0; msg[i] != '\0' && i < max; i++) {
        printf("%c", tolower(msg[i]));
    }

    printf("\n");

    return 0;
}

