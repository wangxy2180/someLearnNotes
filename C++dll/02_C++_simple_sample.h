#pragma once
#include <iostream>
#include <string>
class dll_test {
	
public:
	void print_welcome()
	{
		std::cout << "this is print_welcome,WELCOME!" << std::endl;
	}

	void print_custom(std::string text)
	{
		std::cout << text << std::endl;
	}

};