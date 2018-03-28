 ///
 /// @file    MemCache.h
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-27 20:52:27
 //
 
#ifndef __WD_MEMCACHE_H__
#define __WD_MEMCACHE_H__


#include <string>
#include <list>
#include <unordered_map>
#include <utility>
using std::list;
using std::unordered_map;
using std::pair;
using std::string;

class MemCache
{
public:
	MemCache(size_t capacity = 100);

	void addCache(const pair<string,string>& element);
	
	string query(const string&);

	void reNew();
private:
	size_t _capacity;
	list<pair<string,string>> _cache;
	unordered_map<string,list<pair<string,string>>::iterator> _querys;
};

#endif 
