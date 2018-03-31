///
/// @file    redis.cc
/// @author  sgzed(wunaisong@163.com)
/// @date    2018-03-29 23:19:42
///

#include "redis.h"

#include <string.h>
#include <stdlib.h>

#include <sstream>
#include <iostream>

namespace ccx{

	using std::cout;
	using std::endl;
	using std::stringstream;

#define SETSTRING(key, value) \
	stringstream ss;\
	ss << "SET " << key << " " << value;\
	string s;\
	getline(ss, s);\
	setString(s);

	Redis::Redis()
	{
		_reply = NULL;
	}
	
	void Redis::Connect()
	{
		_context = ::redisConnect("127.0.0.1", 6379);
		if(_context && _context->err)
		{
			cout << "connect redis error" << endl;
			exit(EXIT_FAILURE);    
		}
		cout << "redis Connect success" << endl;
	}

	void Redis::disConnect()
	{
		::redisFree(_context);
		cout << "redis disConnect success" << endl;
	}

	void Redis::setString(const string & data)
	{
		freeReply();
		_reply = static_cast<redisReply*>(::redisCommand(_context, data.c_str()));
		if(!isError())
		{
			if (!(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str,"OK") == 0))
			{
				cout << "Failed to execute SET(string)" << endl;
			}    
		}
	}

	void Redis::setString(const string & key, const string & value)
	{
		freeReply();

		cout << "Redis::setString(const string & key, const string & value)" << endl;
		 
		stringstream ss;

		ss << "SET " << key << " " << value;
		
		string s;
	
		getline(ss, s);
		
		setString(s);
	}

	void Redis::setString(const string & key, const int & value)
	{
		SETSTRING(key, value);
	}

	void Redis::setString(const string & key, const float & value)
	{
		SETSTRING(key, value);
	}

	void Redis::getString(const string & key)
	{
		freeReply();
		_reply = static_cast<redisReply*>(::redisCommand(_context, "GET %s", key.c_str()));
	}

	void Redis::getString(const string & key, string & value)
	{
		getString(key);
		if(!isError() && _reply->type == REDIS_REPLY_STRING)
		{
			value = _reply->str;
		}
	}

	void Redis::getString(const string & key, int & value)
	{
		getString(key);
		if(!isError() && _reply->type == REDIS_REPLY_STRING)
		{
			value = ::atoi(_reply->str);
		}
	}

	void Redis::getString(const string & key, float & value)
	{
		getString(key);
		if(!isError() && _reply->type == REDIS_REPLY_STRING)
		{
			value = ::atof(_reply->str);
		}
	}

	int Redis::rpush(const string& list,const string& value)
	{
		freeReply();
		
		_reply = static_cast<redisReply*>(::redisCommand(_context,"RPUSH %s %s",list.c_str(),
				value.c_str()));

		int ret  = _reply->integer;
		return ret;
	}

	int Redis::listLen(const string& list)
	{
		freeReply();
		_reply = static_cast<redisReply*>(::redisCommand(_context,"LLEN %s",list.c_str()));
		return _reply->integer;
	}

	vector<string> Redis::getList(const string& list)
	{
		int len = listLen(list);
	
		freeReply();

		_reply = static_cast<redisReply*>(::redisCommand(_context,"LRANGE %s %d %d",list.c_str(),
				0,len-1));

		vector<string> res;

		if(_reply->type == REDIS_REPLY_ARRAY)
		{
	    	for(int idx=0; idx<len;++idx)
	    	{
				string tmp(_reply->element[idx]->str);	
				res.push_back(tmp);
			}
		}
		return res;
	}
		
	void Redis::setExpire(const string& key,int time)
	{
		freeReply();
		_reply = static_cast<redisReply*>(::redisCommand(_context,"EXPIRE %s %d",key.c_str(),time));
	}

	void Redis::freeReply()
	{
		if(_reply)
		{
			::freeReplyObject(_reply);
			_reply = NULL;
		}
	}

	bool Redis::isError()
	{
		if(NULL == _reply)
		{
			freeReply();
			disConnect();
			Connect();
			return true;
		}
		return false;
	}

	Redis::~Redis()
	{
		cout << "~Redis()" << endl;
	}
}



