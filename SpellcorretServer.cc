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
	,_cacheManager(_numThreads)
{
	_server.setConnectionCallback(std::bind(&SpellCorrectServer::onConnection,this,_1));

	_server.setMessageCallback(std::bind(&SpellCorrectServer::onMessage,this,_1,_2,_3));

	loop->runEvery(100,std::bind(&MemCacheManager::updata,&_cacheManager));
}

void SpellCorrectServer::start()
{
	_threadPool.start(_numThreads);
	_server.start();
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

	string msg(buf->peek(),buf->peek()+buf->readableBytes());
	buf->retrieveAll();

	MyTask _myTask(msg,_cacheManager);

	LOG_INFO << conn->name() << " recevied " << msg.size() << " bytes, "
		 << "data received at " << time.toString();

	_threadPool.run(std::bind(&MyTask::process,&_myTask,conn));
}













