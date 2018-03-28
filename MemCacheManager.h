 ///
 /// @file    MemCacheManager.h
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-28 16:31:50
 ///

#ifndef __WD_MEMCACHEMANANGER_H__
#define __WD_MEMCACHEMANANGER_H__

#include "MemCache.h"

#include <vector>
using std::vector;

class MemCacheManager
{
public:
	MemCacheManager(size_t size);

	MemCache& operator[](size_t );

	void updata();

	void printForDebug() ;

private:
	vector<MemCache> _memManager;
};

#endif 
