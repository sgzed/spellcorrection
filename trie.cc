///
/// @file    trie.cc
/// @author  sgzed(wunaisong@163.com)
/// @date    2018-03-28 23:01:13
///

#include "trie.h"

#include "EditDistance.h"
#include <queue>
#include <utility>
using std::queue;

//size_t nBytesCode(const char ch)
//{
//	if(ch & (1 << 7))
//	{
//		int nBytes = 1;
//		for(int idx = 0; idx != 6; ++idx)
//		{
//			if(ch & (1 << (6 - idx)))
//			{
//				++nBytes;	
//			}
//			else
//				break;
//		}
//		return nBytes;
//	}
//	return 1;
//}  


trie::trie()
{
	root = new trie_node();
}

trie::~trie()
{
}

void trie::insert_str(string str)
{
	if(root==NULL || str=="")
		return ;
	trie_node* cur_node = root;

	for(size_t i=0;i<str.size();)
	{	
		size_t nBytes = nBytesCode(str[i]);

		string sub_str = str.substr(i,nBytes);
		map<string, trie_node *>::iterator iter = cur_node->child.find(sub_str);

		if (iter == cur_node->child.end())//如果在map中没有找到则插入新节点    
		{    
			trie_node *tmp_node = new trie_node();    
			cur_node->child.insert(std::pair<string, trie_node *>(sub_str, tmp_node));    
			cur_node = tmp_node;    
		}    
		else//如果找到了value即为指向下一个节点的指针    
		{    
			cur_node = iter->second;    
		}    
		i = i + nBytes ;    
	}
	++cur_node->count;
}

void trie::insert_times(string str,size_t cnt) // 插入字符,并且将count值修改为cnt
{
	if(root==NULL || str=="")
		return ;
	trie_node* cur_node = root;

	for(size_t i=0;i<str.size();)
	{	
		size_t nBytes = nBytesCode(str[i]);

		string sub_str = str.substr(i,nBytes);
		map<string, trie_node *>::iterator iter = cur_node->child.find(sub_str);

		if (iter == cur_node->child.end())//如果在map中没有找到则插入新节点    
		{    
			trie_node *tmp_node = new trie_node();    
			cur_node->child.insert(std::pair<string, trie_node *>(sub_str, tmp_node));    
			cur_node = tmp_node;    
		}    
		else//如果找到了value即为指向下一个节点的指针    
		{    
			cur_node = iter->second;    
		}    
		i = i + nBytes ;    
	}
	cur_node->count = cnt;
}

void trie::delete_str(string str)    
{    
	trie_node *find_node = search_str(str);    

	if (find_node)    
	{    
		find_node->count--;    
	}    
}    

trie_node * trie::search_str_pre(string str)    
{    
	if (str == "")    
	{    
		return root;    
	}    
	if (NULL == root )    
	{    
		return NULL;    
	}    

	trie_node *cur_node = root;    

	size_t  i;    
	for ( i = 0; i < str.size(); )    
	{   
		size_t nBytes = nBytesCode(str[i]);
		string sub_str = str.substr(i, nBytes);    
		map<string, trie_node *>::iterator iter = cur_node->child.find(sub_str);    

		if (iter == cur_node->child.end())    
		{    
			return NULL;    
		}    
		else    
		{    
			cur_node = iter->second;    
		}    
		i = i + nBytes;    
	}    

	if (i == str.size())    
	{    
		return cur_node;    
	}    
	else    
	{    
		return NULL;    
	}    
}    

trie_node * trie::search_str(string str)    
{    
	trie_node * find_pre_node = search_str_pre(str);    

	if (find_pre_node != NULL)    
	{    
		if (find_pre_node->count == 0)    
		{    
			return NULL;    
		}    
		else    
		{    
			return find_pre_node;    
		}    
	}   
	return NULL;
}    

void trie::clear()    
{    
	queue<trie_node *> que;    
	que.push(root);    

	while (!que.empty())    
	{    
		for (map<string, trie_node *>::iterator iter = que.front()->child.begin(); iter != que.front()->child.end(); iter++)    
		{    
			que.push(iter->second);    
		}    

		trie_node *del_node = que.front();    
		que.pop();  
		delete del_node;    
	}    
}    

void trie::add_str(trie_node *node, string pre_str, vector<string> &ret)    
{    

	for (map<string, trie_node *>::iterator iter = node->child.begin(); iter != node->child.end(); iter++)    
	{    
		add_str(iter->second, pre_str + iter->first, ret);    
	}    

	if (node->count != 0)    
	{    
		ret.push_back(pre_str);    
	}    
}    

vector<string> trie::get_str_pre(string str)    
{    
	vector<string> ret;    
	trie_node *find_node = search_str_pre(str);    

	if (find_node != NULL)    
	{    
		add_str(find_node, str, ret);    
	}    
	return ret;    
}   

void trie::show()    
{    
	queue<trie_node *> que;    
	que.push(root);    

	while (!que.empty())    
	{    
		for (map<string, trie_node *>::iterator iter = que.front()->child.begin(); iter != que.front()->child.end(); iter++)    
		{    
			que.push(iter->second);    
		}    

		trie_node *show_node = que.front();    
		for(auto iter : show_node->child)	
		{
			std::cout << iter.first << " " ;
		}
		std::cout << std::endl;
		que.pop();   
	}    
}    










