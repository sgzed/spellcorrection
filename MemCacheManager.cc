 ///
 /// @file    MemCacheManager.cc
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-28 16:35:54
 ///

#include "MemCacheManager.h"

MemCacheManager::MemCacheManager(size_t size)
{
	_memManager.resize(size);
}

void MemCacheManager::updata()
{
	auto  first  = _memManager.begin();	

	for(auto& iter : _memManager)
	{
		iter = *first;

	//	iter.reNew();
	}
}

void MemCacheManager::printForDebug() 
{
	for(auto& iter : _memManager)
	{
		iter.reNew();
	}
}

MemCache&  MemCacheManager::operator[](size_t idx)
{
	return _memManager[idx];
}




