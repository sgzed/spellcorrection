 ///
 /// @file    MyTask.h
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-27 15:17:36
 ///

#ifndef __WD_MYTASK_H__
#define __WD_MYTASK_H__

#include "EditDistance.h"
#include "MyConf.h"
#include "MemCache.h"
#include "MemCacheManager.h"
#include <muduo/net/TcpConnection.h>
#include "IndexProducer.h"
#include <muduo/base/CurrentThread.h>

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
	MyTask(const string& query,MemCacheManager& cacheManager)
		:_query(query)
		 ,_cacheManager(cacheManager)
	{	
	}

	priority_queue<MyResult,vector<MyResult>,MyCompare> getPriorityQueue()
	{
		return _priqueue;
	}

	void process(const muduo::net::TcpConnectionPtr& conn)
	{
		cout << "subThread :" << muduo::CurrentThread::t_threadName << endl;
		
		size_t idx = str2int(muduo::CurrentThread::t_threadName)-1;

		MemCache& cache = _cacheManager[idx];

		string s = cache.query(_query);

		if(s!="")
			conn->send(s);
		else 
			getFromIndex(conn,cache);
	}

	void getFromIndex(const muduo::net::TcpConnectionPtr& conn,MemCache& cache)
	{
//		std::unordered_map<string,std::set<int>> mapIndex = IndexProducer::getInstance()->getIndex();
		
//		vector<pair<string,int>> dict = IndexProducer::getInstance()->getDict(); 
	
		trie indexTrie  = IndexProducer::getInstance()->getTrie();	

		string ch;

		vector<string> seach;

		for(size_t idx = 0; idx < _query.size();)
		{
			size_t nBytes = nBytesCode(_query[idx]);
			
			ch = _query.substr(idx,nBytes);
	
			vector<string> tmp = indexTrie.get_str_pre(ch);
			seach.insert(seach.end(),tmp.begin(),tmp.end());

			idx+=nBytes;
		}
		
		for(auto it : seach)
		{
			cout << "query = " << _query  << "  " << "it = "<< it <<endl;
			size_t distance = editDistance(_query,it);
			//cout << "the pos in vector is " << it << " and distance is " << distance << endl;
			if(distance <=3 )
			{
				MyResult result ;	
				result._word = it;
				result._iFreq  = indexTrie.search_str(it)->count;
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
				res += " ";
				_priqueue.pop();
				--cnt;
			}
			res += "\n";
			cache.addCache(make_pair(_query,res));
			cache.reNew();
			conn->send(res);
		}
	}

private:
	string _query;
	MemCacheManager& _cacheManager;
	priority_queue<MyResult,vector<MyResult>,MyCompare> _priqueue; 
};


#endif
