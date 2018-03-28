 ///
 /// @file    MyTask.cc
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-28 16:48:02
 ///

#include "MyTask.h"
#include <sstream>

int str2int(const string& str)
{
	std::istringstream iss(str);
	int number;
	iss >> number;
	return number;
}
