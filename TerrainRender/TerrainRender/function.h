#pragma once

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <vector>
#include <string>

using namespace std;

///string的split功能
///返回string容器，若没有对应分隔符则返回原string
vector<string> str_split(const string str, const string pattern)
{
	string::size_type pos = 0;
	vector<string> vec;
	string buf(str);
	int size = str.size();
	for (int i = 0; i < size; i = pos + pattern.size())//注意这一步，原写法是i++，在if条件最后一步为i = pos + pattern.size() - 1
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