 ///
 /// @file    main.cc
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-26 19:36:08
 ///


#include "SpellcorretServer.h"
#include "MyConf.h"

#include "IndexProducer.h"
#include "DictProducer.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
//#include "Configuration.h"

uint16_t str2unshort(string& str)
{
	std::istringstream iss(str);
	uint16_t unshort;
	iss >> unshort ;
	return unshort;
}

int main()
{
//	Configuration f("./conf/my.conf");

	MyConf::getInstance()->show();
	std::map<string,string> confMap = MyConf::getInstance()->getConfigMap();
	
	string ip  = confMap["my_ip"];
	string port = confMap["my_port"];
	string store = confMap["my_dict"];
	string numThreads = confMap["my_thread_num"];
	
	DictProducer dict("./english.txt");

	dict.build_dict();

	dict.store_dict(store.c_str());


	LOG_INFO  << "pid = " << getpid(); 
	muduo::net::EventLoop loop;

	LOG_INFO << "ip = " << ip <<  " port = " << str2unshort(port);

	muduo::net::InetAddress listenAddr(ip,str2unshort(port));
	SpellCorrectServer server(&loop,listenAddr,str2unshort(numThreads));
	server.start();
	loop.loop();

	return 0;
}


