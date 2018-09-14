#pragma once

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <vector>
#include <string>

using namespace std;

///string��split����
///����string��������û�ж�Ӧ�ָ����򷵻�ԭstring
vector<string> str_split(const string str, const string pattern)
{
	string::size_type pos = 0;
	vector<string> vec;
	string buf(str);
	int size = str.size();
	for (int i = 0; i < size; i = pos + pattern.size())//ע����һ����ԭд����i++����if�������һ��Ϊi = pos + pattern.size() - 1
	{
		pos = buf.find(pattern, i);
		if (pos == -1)
		{
			string temp = buf.substr(i, size - i);
			vec.push_back(temp);
			return vec;
		}
		else if (pos < size && pos != i)
		{
			string temp = buf.substr(i, pos - i);
			vec.push_back(temp);
		}
	}
	return vec;
}

#endif