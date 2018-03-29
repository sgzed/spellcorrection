 ///
 /// @file    EditDistance.h
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-27 14:01:56
 ///

#ifndef __WD_EDITDISTANCE_H__
#define __WD_EDITDISTANCE_H__

#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

size_t nBytesCode(const char ch);

size_t length(const string& str);

size_t min(size_t a,size_t b,size_t c);

size_t editDistance(const string& lhs,const string& rhs);

#endif 
