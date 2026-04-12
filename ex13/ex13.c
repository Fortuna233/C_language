#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("ERROR: 0 argument.\n");
		return 1;
	}
	for (int j = 1; j < argc; j++)
	{
		// T2
		char letter;
		for (int i = 0; (letter = argv[j][i]) != '\0'; i++)
		{
			// T1
			if (letter < 97)
			{
				letter += 32;
			}
			switch (letter)
			{
			case 'a':
				printf("%d: 'A'\n", i);
				break;

			case 'e':
				printf("%d: 'E'\n", i);
				break;

			case 'i':
				printf("%d: 'I'\n", i);
				break;

			case 'o':
				printf("%d: 'O'\n", i);
				break;

			case 'u':
				printf("%d: 'U'\n", i);
				break;
			case 'y':
				if (i > 2)
				{
					printf("%d: 'Y'\n", i);
				}
				break;
				// T5
				// 效果：
				// 无论 i > 2 是否成立，执行完 if 判断后，一定会执行 break，直接退出 switch，绝对不会执行 default。
				// 结论：y 永远不会触发 default（不会打印 y is not a vowel）
				// 问题 2：把 break 移进 if 代码块里面，会发生什么？
				// 效果：
				// C 语言 switch 会发生穿透（fall-through）：
				// i > 2：正常打印 + break → 退出 switch（正常）
				// i <= 2：不打印，没有 break → 直接执行下一个分支（default）
				// 验证：
				// 字符 y + i=1：不打印，穿透到 default → 输出 1: y is not a vowel
				// 字符 y + i=3：打印Y + break → 正常退出
			default:
				printf("%d: %c is not a vowel\n", i, letter);
			}

			// T4
			// // 👇 替换为 if-else 语句，逻辑和原switch完全一致
			// if (letter == 'a') {
			// 	printf("%d: 'A'\n", i);
			// }
			// else if (letter == 'e') {
			// 	printf("%d: 'E'\n", i);
			// }
			// else if (letter == 'i') {
			// 	printf("%d: 'I'\n", i);
			// }
			// else if (letter == 'o') {
			// 	printf("%d: 'O'\n", i);
			// }
			// else if (letter == 'u') {
			// 	printf("%d: 'U'\n", i);
			// }
			// else if (letter == 'y') {
			// 	if (i > 2)
			// 	{
			// 		printf("%d: 'Y'\n", i);
			// 	}
			// 	// 对应原switch的break（if-else无穿透，无需break）
			// }
			// else {
			// 	printf("%d: %c is not a vowel\n", i, letter);
			// }
		}
	}

	return 0;
}
