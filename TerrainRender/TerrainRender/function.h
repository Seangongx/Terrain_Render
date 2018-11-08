#pragma once
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <vector>
#include <string>

///string��split����
///����string��������û�ж�Ӧ�ָ����򷵻�ԭstring
std::vector<std::string> str_split(const std::string str, const std::string pattern)
{
	//string::size_type pos = 0;
	int pos = 0;
	std::vector<std::string> vec;
	std::string buf(str);
	int size = str.size();
	for (int i = 0; i < size; i = pos + pattern.size())//ע����һ����ԭд����i++����if�������һ��Ϊi = pos + pattern.size() - 1
	{
		pos = buf.find(pattern, i);
		if (pos == -1)
		{
			std::string temp = buf.substr(i, size - i);
			vec.push_back(temp);
			return vec;
		}
		else if (pos < size && pos != i)
		{
			std::string temp = buf.substr(i, pos - i);
			vec.push_back(temp);
		}
	}
	return vec;
}

#endif