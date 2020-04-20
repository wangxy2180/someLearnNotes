#include <iostream>
#include "02_C++_simple_sample.h"
#include <cstring>
/*
对于C++，则需要加上extern "C"{}这样的语句
让这部分代码按C语言的方式进行编译
*/
extern "C"
{
	__declspec(dllexport)
		int sum(int a, int b)
	{
		std::cout << "result is " <<a + b << std::endl;
		return a + b;
	}

}


extern "C"
{
	//这是一种调用类中函数的方法，还可以把类中的每一个函数都 __declspec(dllexport)
	dll_test dll__test;

	__declspec(dllexport)
	void print_welcome_c()
	{
		dll__test.print_welcome();
	}

	/*
	在Python中调用时，需要这样写
	print_custom_c('我是输入'.encode('utf8'))或者
	print_custom_c('我是输入'.encode('ascii'))
	如果只有字符串，不转编码，只会输出第一个字符
	*/
	__declspec(dllexport)
	void print_custom_c(char* str )
	{
		dll__test.print_custom(str);
	}

	//这个我做不出来了，留个坑
	//__declspec(dllexport)
	//	char* ret_str()
	//{
	//	char aa[] = "aaaaa";
	//	return aa;
	//}
}
