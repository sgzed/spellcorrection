 ///
 /// @file    SpellcorretServer.cc
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-26 22:28:27
 ///

#include "SpellcorretServer.h"

#include "EditDistance.h"
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <functional>

#define MUDUO_STD_STRING 

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

SpellCorrectServer::SpellCorrectServer(muduo::net::EventLoop* loop,
		const muduo::net::InetAddress& listenAddr,unsigned short numThreads)
	:
	_server(loop,listenAddr,"SpellCorrectServer")
	 ,_numThreads(numThreads)
	,_threadPool("")
	,_mutex()
{
	_server.setConnectionCallback(std::bind(&SpellCorrectServer::onConnection,this,_1));

	_server.setMessageCallback(std::bind(&SpellCorrectServer::onMessage,this,_1,_2,_3));

	for(int idx=0;idx != _numThreads;++idx)
	{
		ccx::Redis redis = ccx::Redis();
		_redisque.push_back(redis);
	}

}

void SpellCorrectServer::start()
{
	_threadPool.start(_numThreads);
	_server.start();
	
	for(auto& iter : _redisque)
		iter.Connect();

}

void  SpellCorrectServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
	LOG_INFO << "SpellCorrectServer : " << conn->peerAddress().toIpPort() << " -> " 
		<< conn->localAddress().toIpPort() << " is "
		 << (conn->connected() ? "UP" : "DOWN");
}

void SpellCorrectServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
		muduo::net::Buffer * buf,muduo::Timestamp time)
{
	
//	string msg = static_cast<string>(buf->retrieveAllAsString());

	string msg(buf->peek(),buf->peek()+buf->readableBytes()-1);
	buf->retrieveAll();

	ccx::Redis redis;
	
	{
		muduo::MutexLockGuard lock(_mutex);
	
		redis = _redisque.front();
	
		_redisque.pop_front();
	}

	MyTask _myTask(msg,redis);

	LOG_INFO << conn->name() << " recevied " << msg.size() << " bytes, "
		 << "data received at " << time.toString() << " body is " << msg;

	_threadPool.run(std::bind(&MyTask::process,&_myTask,conn));

	{
		muduo::MutexLockGuard lock(_mutex);
		_redisque.push_back(redis);
	}
}














