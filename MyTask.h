 ///
 /// @file    MyTask.h
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-27 15:17:36
 ///

#ifndef __WD_MYTASK_H__
#define __WD_MYTASK_H__

#include "EditDistance.h"
#include "MyConf.h"
#include <muduo/net/TcpConnection.h>
#include "IndexProducer.h"
#include <muduo/base/CurrentThread.h>
#include "redis.h"

#include <utility>
#include <string>
#include <set>
#include <queue>
#include <vector>

using std::vector;
using std::string;
using std::priority_queue;
using std::make_pair;

int str2int(const string& str);

struct MyResult
{
	string _word;
	size_t _iFreq;
	size_t _iDist;
};

struct MyCompare
{
	 bool operator()(const MyResult& lhs,const MyResult& rhs)
	{ 
		if(lhs._iDist > rhs._iDist)	
			return true;
		else if(lhs._iDist==rhs._iDist && lhs._iFreq<rhs._iFreq)
			return true;
		else if(lhs._iDist==rhs._iDist && lhs._iFreq==rhs._iFreq &&
				lhs._word > rhs._word)
			return true;
		else
			return false;
	}
};

class MyTask
{
public:
	MyTask(const string& query,ccx::Redis& redis)
		:_query(query)
		 ,_redis(redis)
	{	
	}

	priority_queue<MyResult,vector<MyResult>,MyCompare> getPriorityQueue()
	{
		return _priqueue;
	}

	void process(const muduo::net::TcpConnectionPtr& conn)
	{
		cout << "subThread :" << muduo::CurrentThread::t_threadName << endl;
		
	//	size_t idx = str2int(muduo::CurrentThread::t_threadName)-1;

		if(_redis.listLen(_query) > 0)
		{
		   	vector<string> res = _redis.getList(_query);
			string s ;

			cout << "found in redis  len = " << _redis.listLen(_query) << endl;;
			for(auto iter : res)
			{
				cout << iter << " " ;
				s+=iter;
				s+=" ";
			}
			cout << endl;
			s+="\n";
			conn->send(s);
			_redis.setExpire(_query,10);
		}
		else 
			getFromIndex(conn);
	}

	void getFromIndex(const muduo::net::TcpConnectionPtr& conn)
	{
		std::unordered_map<string,std::set<int>> mapIndex = IndexProducer::getInstance()->getIndex();
		
		vector<pair<string,int>> dict = IndexProducer::getInstance()->getDict(); 
	
//		for(auto& iter : dict)
//		{
//			cout << iter.first << "--> " << iter.second << endl;
//		}

		std::set<int> lineset; 
		
		string ch;

		for(size_t idx = 0; idx < _query.size();)
		{
			size_t nBytes = nBytesCode(_query[idx]);
			
			ch = _query.substr(idx,nBytes);

			idx+=nBytes;
		
			lineset.insert(mapIndex[ch].begin(),mapIndex[ch].end());
		}
		
		for(auto it : lineset)
		{
			cout << "query = " << _query  << "   " << "dict[it].first = "<<dict[it].first<<endl;
			size_t distance = editDistance(_query,dict[it].first);
			//cout << "the pos in vector is " << it << " and distance is " << distance << endl;
			if(distance <=3 )
			{
				MyResult result ;	
				result._word = dict[it].first;
				result._iFreq  = dict[it].second;
				result._iDist = distance;
				_priqueue.push(result);
			}
		}
	
		if(_priqueue.empty())
		{
			string msg = "no answer!\n" ;
			conn->send(msg);
		}
		else
		{
			int cnt = 3 ;
			string res;
			while(!_priqueue.empty() && cnt>0)
			{	
			    res +=_priqueue.top()._word;
				_redis.rpush(_query,_priqueue.top()._word);
				res += " ";
				_priqueue.pop();
				--cnt;
			}
			_redis.setExpire(_query,10);
			res += "\n";
			conn->send(res);
		}
	}

private:
	string _query;
	ccx::Redis& _redis;
	priority_queue<MyResult,vector<MyResult>,MyCompare> _priqueue; 
};


#endif
