///
/// @file    redis.h
/// @author  sgzed(wunaisong@163.com)
/// @date    2018-03-29 23:17:31
///

#ifndef __REDIS_H__
#define __REDIS_H__

#include <hiredis/hiredis.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace ccx{

	class Redis
	{
		public:
			Redis();
		public:
			void Connect();
			void disConnect();
		public:
			void setString(const string & key, const string & value);
			void setString(const string & key, const int & value);
			void setString(const string & key, const float & value);
		
			int  rpush(const string& list,const string& value);
			int  rpush(const string& list,const string& value,int time);
			int  listLen(const string& list);	
			vector<string> getList(const string& list);

			void setExpire(const string& key,int time);
		private:
			void setString(const string & data);
		public:
			void getString(const string & key, string & value);
			void getString(const string & key, int & value);
			void getString(const string & key, float & value);
		private:
			void getString(const string & key);
		private:
			void freeReply();
			bool isError();
		private:
			redisContext * _context;
			redisReply * _reply;
	};
}

#endif
