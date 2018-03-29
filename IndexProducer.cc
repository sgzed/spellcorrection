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
//vector<pair<string,int>> IndexProducer::_dict ;
//unordered_map<string,set<int>> IndexProducer::_index;
trie IndexProducer::_trie;

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
		size_t value;
		ss >> key >> value;
		_trie.insert_times(key,value);
	}
	ifs.close();
}

void IndexProducer::init()
{
	gInstance = new IndexProducer();

	readFromData();
	//ifstream ifs("./data/index.dat");
	::atexit(destroy);
}

void IndexProducer::destroy()
{
	::delete gInstance;
}

void IndexProducer::show() const 
{
	_trie.show();
}























