 ///
 /// @file    trie.h
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-28 22:51:13
 ///

#ifndef __WD_TRIE_H__
#define __WD_TRIE_H__

#include <string>    
#include <map>    
#include <vector>    
#include <iostream>

using std::map;
using std::string;
using std::vector;

struct trie_node
{
	trie_node()
	{
		count = 0;
	}
	int count;  // 表示以该汉字结束的字符串个数
	map<string,trie_node*> child; //key为当前汉字，实值为后面汉字的指针
};

class trie
{
public:
	trie();
	~trie();
	void insert_str(string str); // 插入字符
	void insert_times(string str,size_t cnt); // 插入字符,并且将count值修改为cnt
	trie_node* search_str(const string str); //查询字符串
	trie_node* search_str_pre(const string str);   //查询前缀
	void delete_str(string str);    //删除
	vector<string> get_str_pre(string str); //返回前缀为str的字符串
	void clear();
	void show();

private:
	void add_str(trie_node* root,string pre_str,vector<string>& ret); //递归添加

	struct trie_node* root;
};

#endif






















