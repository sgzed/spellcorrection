 ///
 /// @file    MemCache.cc
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-27 20:52:27
 ///

#include "MemCache.h"
#include <iostream>
using std::cout;
using std::endl;

MemCache::MemCache(size_t capacity)
	:_capacity(capacity)
{
}

void MemCache::addCache(const pair<string,string>& element)
{
	auto iter = _querys.find(element.first);	

	if(iter !=_querys.end())	
	{
		_cache.erase(iter->second);
		_cache.push_front(element);
		_querys[element.first] = _cache.begin();
	}
	else
	{
		if(_cache.size()==_capacity)	
		{
			_cache.pop_back();
		}
		_cache.push_front(element);
		_querys[element.first] = _cache.begin();
	}
}

void MemCache::reNew()
{
	for(auto & iter : _cache)
	{
		cout << iter.first << ":" << iter.second << endl;;
	}

	cout << " ----------------------- " << endl;
}


