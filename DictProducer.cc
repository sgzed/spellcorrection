 ///
 /// @file    DictProducer.cc
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-26 23:20:20
 ///

#include "DictProducer.h"
#include <fstream>
#include <cctype>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
using std::ifstream;
using std::ofstream;
using std::istringstream;


DictProducer::DictProducer(const string& dir)
	:_dir(dir)
{

}

void DictProducer::build_dict()
{
	ifstream ifs(_dir);
	string line;
	string word; 

	while(getline(ifs,line))
	{
		istringstream iss(line);
	
		while(iss >> word)
		{
			string useful;
			std::copy_if(word.begin(),word.end(),back_inserter(useful),isalpha);

			for(auto& iter : useful)
			{
				if(isupper(iter))
					iter = std::tolower(iter);
			}

			if(useful != "")
			{
//				std::cout << useful <<  std::endl;
				++_dic[useful];
			}
		}
	}
}

void DictProducer::store_dict(const char* filepath)
{
	ofstream ofs(filepath);
	
	for(auto iter : _dic)
	{
		ofs << iter.first  << " " << iter.second << "\n"; 
	}
}

void DictProducer::show_files() const
{

}

void get_files()
{

}

void DictProducer::push_dict(const string& word)
{

}

