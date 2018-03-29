///
/// @file    IndexProducer.cc
/// @author  sgzed(wunaisong@163.com)
/// @date    2018-03-27 09:55:08
///

#include "IndexProducer.h"
#include "MyConf.h"

#include <assert.h>
#include <stdlib.h>

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
using std::map;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

pthread_once_t IndexProducer::_ponce = PTHREAD_ONCE_INIT;
IndexProducer * IndexProducer::gInstance = NULL;
vector<pair<string,int>> IndexProducer::_dict ;
unordered_map<string,set<int>> IndexProducer::_index;

IndexProducer* IndexProducer::getInstance()
{
	pthread_once(&_ponce,IndexProducer::init);
	assert(gInstance != NULL);
	return gInstance;
}

void IndexProducer::readFromData()
{

	map<string,string> confMap = MyConf::getInstance()->getConfigMap();

	string path =  confMap["my_dict"];

	ifstream ifs(path);

	if(!ifs.good())
	{
		cout << "open " << path << " failed\n";
		return ;
	}

	string line;
	while(getline(ifs,line))
	{
		std::stringstream ss(line);
		string key;
		int value;
		ss >> key >> value ;
		_dict.push_back(make_pair(key,value));
	}
	ifs.close();
}

void IndexProducer::init()
{
	gInstance = new IndexProducer();

	readFromData();

	for(size_t idx = 0; idx != _dict.size();++idx)
	{
		string key;
		string word = _dict[idx].first;

		for(size_t iidx = 0; iidx!=word.size();++iidx)	
		{
			char ch = word[iidx];

			if(ch&(1<<7))
			{
				// 存储utf-8编码的中文字符
				if((ch&0xF0)==0xC0 || (ch&0xF0)==0xD0)
				{
					key = word.substr(iidx,2);
					++iidx;
				}
				else if((ch&0xF0)==0xE0)
				{
					key = word.substr(iidx,3);
					iidx+=2;
				}
				else if((ch & 0xFF) == 0xF0 || 
						(ch & 0xFF) == 0xF1 || 
						(ch & 0xFF) == 0xF2 || 
						(ch & 0xFF) == 0xF3 || 
						(ch & 0xFF) == 0xF4 || 
						(ch & 0xFF) == 0xF5 || 
						(ch & 0xFF) == 0xF6 || 
						(ch & 0xFF) == 0xF7)
				{
					key = word.substr(iidx,4);
					iidx+=3;
				}
				else if( (ch & 0xFF) == 0xF8 ||
						(ch & 0xFF) == 0xF9 || 
						(ch & 0xFF) == 0xFA || 
						(ch & 0xFF) == 0xFB  ) 
				{	
					key = word.substr(iidx,5);
					iidx+=4;
				}
				else if((ch&0xFF)==0xFC)
				{
					key = word.substr(iidx,6);
					iidx+=5;
				}
		     }
			else 
			{
				key = word.substr(iidx,1);
			}
			//	cout << s << endl;
			auto element = _index.find(key);
			if(element != _index.end())
			{
				element->second.insert(idx);
			}
			else
				// 存放的包含字符a~z的字符串在vector中的下标
				_index.insert(make_pair(key,set<int>{static_cast<int>(idx)}));
		}
	}
	//ifstream ifs("./data/index.dat");
	::atexit(destroy);
}

void IndexProducer::destroy()
{
	::delete gInstance;
}

void IndexProducer::show() const 
{
	for(auto& iter : _index)
	{
		cout << iter.first <<  " : ";
		for(auto& it : iter.second)
			cout << it << " ";
		cout << endl;
	}
}























