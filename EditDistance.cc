 ///
 /// @file    EditDistance.cc
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-27 17:16:15
 ///

#include "EditDistance.h"

//获取一个字节高位开头为1的个数
size_t nBytesCode(const char ch)
{
	if(ch&(1<<7))
	{
		int nBytes = 1;
		for(int idx=0;idx!=6;++idx)
		{
			if(ch&(1<<(6-idx)))
				++nBytes;
			else
				break;
		}
		cout << nBytes << endl;
		return nBytes;
	}
	return 1;
}

size_t length(const string& str)
{
	size_t ilen = 0;
	for(size_t idx=0;idx!=str.size();++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx+=(nBytes-1);
		++ilen;
	}
	return ilen;
}

size_t min(size_t a,size_t b,size_t c)
{
	size_t tmp = a<b?a:b;
	size_t ret =  c<tmp?c:tmp;
//	cout << ret;
	return ret;
}

size_t editDistance(const string& lhs,const string& rhs)
{
	size_t lhs_len = length(lhs);
	size_t rhs_len =  length(rhs);

	size_t editDist[lhs_len+1][rhs_len+1];

	for(size_t idx = 0;idx<=lhs_len;++idx)
		editDist[idx][0] = idx;

	for(size_t idx = 0;idx<=rhs_len;++idx)
		editDist[0][idx] = idx;

	//for(size_t row=1; row<=lhs_len ;++row)
	//{
	//	for(size_t col=1;col<=rhs_len;++col)
	//	{
	//		int cost = lhs[row-1]==rhs[col-1]?0:1;

	//		int deletion = editDist[row-1][col]+1;
	//		int insertion = editDist[row][col-1]+1;
	//		int substitution = editDist[row-1][col-1]+cost;
	//		
	//		editDist[row][col] = min(deletion,insertion,substitution);
	////		cout << editDist[row][col]  << " ";
	//	}
    //	cout << endl;
	// }
	string sublhs,subrhs;

	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = nBytesCode(lhs[lhs_idx]);
		sublhs = lhs.substr(lhs_idx, nBytes);
		lhs_idx += (nBytes - 1);

	  for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
	  {
		nBytes = nBytesCode(rhs[rhs_idx]);
		subrhs = rhs.substr(rhs_idx, nBytes);
		rhs_idx += (nBytes - 1);
		if(sublhs == subrhs)
		{
			editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
		}
		else
		{
			editDist[dist_i][dist_j] = min(
					editDist[dist_i][dist_j-1]+1,
					editDist[dist_i-1][dist_j]+1,
					editDist[dist_i-1][dist_j-1]+1);
		}
	  }
	}

	return editDist[lhs_len][rhs_len];
}

