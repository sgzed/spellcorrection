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

#include <string>
#include <set>
#include <queue>
#include <vector>

using std::vector;
using std::string;
using std::priority_queue;

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
	MyTask(const string& query)
		:_query(query)
	{	
	}

	priority_queue<MyResult,vector<MyResult>,MyCompare> getPriorityQueue()
	{
		return _priqueue;
	}

	void process(const muduo::net::TcpConnectionPtr& conn)
	{
		std::unordered_map<string,std::set<int>> mapIndex = IndexProducer::getInstance()->getIndex();
		
		vector<pair<string,int>> dict = IndexProducer::getInstance()->getDict(); 
		
		std::set<int> lineset; 
		
		for(auto iter : _query)
		{
			string s(1,iter);

			lineset.insert(mapIndex[s].begin(),mapIndex[s].end());

		}
		for(auto it : lineset)
		{
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
			while(!_priqueue.empty())
			{	
				string res =_priqueue.top()._word + " ";
				conn->send(res);
				_priqueue.pop();
			}
			conn->send("\n");
		}
	}

private:
	string _query;
	priority_queue<MyResult,vector<MyResult>,MyCompare> _priqueue; 
};


#endif
