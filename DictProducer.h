 ///
 /// @file    DictProducer.h
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-26 23:14:20
 ///

#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include <map>
#include <string>
#include <vector>
using std::vector;
using std::string;

class DictProducer
{
public:
	DictProducer(const string& dir);

	void build_dict();

	void store_dict(const char* filepath);

	void show_files() const;

	void get_files();

	void push_dict(const string& word);

private:
	string _dir ;
	vector<string > _files;
	std::map<string,int> _dic;
};

#endif
