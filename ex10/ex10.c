#include <stdio.h>

int main(int argc, char *argv[])
{
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas", NULL
    };
    for(int i = 0; i < argc; i++) {
        printf("arg %d: %s\n", i, argv[i]);
    }
	int num_states = 5;

    for(int i = 0; i < num_states; i++) {
        printf("state %d: %s\n", i, states[i]);
        printf("state %d: %p\n", i, states[i]);
		// (null), addr= (nil)
    }

	// T1
	// for(初始化；条件判断；运算）{}
	//
	//
	// T2
	for(int x = 0, y = 0; x < 10 && y < 10; x++, y++) {
		printf("x: %d, y: %d\n", x, y);
	}

	// T3
	// NULL: 直接打印将崩溃，只有非空才会打印。将打印出（null），地址为（nil）
	//
	// T4
	argv[0] = states[0];
	printf("argv[0]: %s\n", argv[0]);	
	states[1] = argv[1];
	printf("states[1]: %s\n", argv[1]);	


    return 0;
}
