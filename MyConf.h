 ///
 /// @file    Singletion.h
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-26 20:11:06
 

#ifndef __WD_SINGLETON_H__
#define __WD_SINGLETON_H__

#include  <muduo/base/noncopyable.h>

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#include <map>
#include <string>
using std::map;
using std::string;

class MyConf  :  muduo::noncopyable
{
public:
	static MyConf* getInstance()
	{
		pthread_once(&_ponce,&MyConf::init);
		assert(_value != NULL);
		return _value;
	}

	static void init();
	
	void show(); 

	map<string,string>& getConfigMap();

private:
	MyConf()
	{}

	~MyConf()
	{} 

	static void  destroy()
	{ delete _value;}

	static map<string,string> _configMap;
	static pthread_once_t _ponce; 
	static MyConf* _value;
};



#endif
