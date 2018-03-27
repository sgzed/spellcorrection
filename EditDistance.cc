 ///
 /// @file    EditDistance.cc
 /// @author  sgzed(wunaisong@163.com)
 /// @date    2018-03-27 17:16:15
 ///

#include "EditDistance.h"

size_t min(size_t a,size_t b,size_t c)
{
	size_t tmp = a<b?a:b;
	size_t ret =  c<tmp?c:tmp;
//	cout << ret;
	return ret;
}


size_t editDistance(const string& lhs,const string& rhs)
{
	size_t lhs_len = lhs.size();
	size_t rhs_len =  rhs.size();

	size_t editDist[lhs_len+1][rhs_len+1];

	for(size_t idx = 0;idx<=lhs_len;++idx)
		editDist[idx][0] = idx;

	for(size_t idx = 0;idx<=rhs_len;++idx)
		editDist[0][idx] = idx;

	for(size_t row=1; row<=lhs_len ;++row)
	{
		for(size_t col=1;col<=rhs_len;++col)
		{
			int cost = lhs[row-1]==rhs[col-1]?0:1;

			int deletion = editDist[row-1][col]+1;
			int insertion = editDist[row][col-1]+1;
			int substitution = editDist[row-1][col-1]+cost;
			
			editDist[row][col] = min(deletion,insertion,substitution);
	//		cout << editDist[row][col]  << " ";
		}
	//	cout << endl;
	}
	return editDist[lhs_len][rhs_len];
}

