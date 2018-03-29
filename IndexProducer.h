 ///
 /// @file    IndexProducer.h
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-27 09:43:02
 ///
 
#ifndef __WD_INDEXPRODUCER_H__
#define __WD_INDEXPRODUCER_H__

#include "trie.h"
#include <pthread.h>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
using std::unordered_map;
using std::pair;
using std::vector;
using std::string;
using std::set;

class IndexProducer
{
public:
	static IndexProducer* getInstance();
	
	static trie& getTrie()
	{
		return _trie;
	}
	
	void show() const ;
private:
	IndexProducer() { }
	~IndexProducer() {} 
	static void readFromData();
	static void init();

	static void destroy();
private:
	static  pthread_once_t _ponce;
	static IndexProducer* gInstance;
	//static vector<pair<string,int>>  _dict;
	//static unordered_map<string,set<int>> _index;
	static trie _trie;
};
#endif
