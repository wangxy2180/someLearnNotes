/*
对于普通的C语言，只需加上那一句就够了
*/


/*
__declspec(dllexport)用于Windows中的动态库中，
声明导出函数、类、对象等供外面调用
*/
#include <stdio.h>
__declspec(dllexport)
int sum(int a, int b)
{
	return a + b;
}

__declspec(dllexport)
int minus(int a, int b)
{
	return a - b;
}

__declspec(dllexport)
void print_welcome()
{
	printf("welcome to learn dll");
}