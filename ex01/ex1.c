#include<stdio.h>
//x1.c:3:1: error: expected identifier or ‘(’ before ‘{’ token
//    3 | {
//      | ^
//make: *** [<内置>：ex1] 错误 1
int main()  //删除括号中内容，对结果无影响
{
	puts("Hello world.");
	
	puts("wuhu");
	puts("wuhu");
	puts("wuhu");
	puts("wuhu");
	return 0;      //删除本行对结果无影响
}
