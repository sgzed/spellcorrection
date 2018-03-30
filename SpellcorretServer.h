 ///
 /// @file    SpellcorretServer.cc
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-26 22:11:09
 ///
 
#ifndef __WD_SPELLCORRECTSERCER_H__
#define __WD_SPELLCORRECTSERCER_H__

#include "MyTask.h"
#include "redis.h"
#include <muduo/base/Thread.h>
#include <muduo/base/ThreadPool.h>
#include <muduo/net/TcpServer.h>

class SpellCorrectServer
{
public:
	SpellCorrectServer(muduo::net::EventLoop* ,const muduo::net::InetAddress&,unsigned short);

	void start();

	~SpellCorrectServer(){}

private:
	void onConnection(const muduo::net::TcpConnectionPtr& conn);
	
	void onMessage(const muduo::net::TcpConnectionPtr& conn,
			muduo::net::Buffer* buf,
			muduo::Timestamp time);

	//void processRequest(muduo::TcpConnectionPtr& conn,const string& str);

	//MyTask _myTask;
	muduo::net::TcpServer _server;
	unsigned short _numThreads;
	muduo::ThreadPool _threadPool;
	ccx::Redis _redis;
};

#endif 
