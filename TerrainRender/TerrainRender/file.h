#ifndef __FILE_H__
#define __FILE_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "function.h"
using namespace std;

int f_get_rows(const string filename)
{
	int count = 0;
	string temp;
	fstream fin(filename, ios::in);
	if (!fin)
	{
		cout << "The file is not exist!" << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		while (getline(fin, temp, '\n'))
		{
			count++;
		}
		fin.close();
		return count;
	}
}

int f_get_columns(const string filename)
{
	string rowstring;
	vector<string> temp;
	ifstream fin(filename, ios::in);

	if (!fin) {
		cout << "The file is not exist!" << endl;
		return -1;
	}
	getline(fin, rowstring, '\n');
	temp = str_split(rowstring, " ");
	fin.close();
	return temp.size();
}

#endif

/*
int f_get_rows(const string filename)
{
	int count = 0;
	string temp;
	fstream fp;
	fp.open(filename, ios::in);
	if (!fp.is_open())
	{
		cout << "file read error!" << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		while(getline(fp, temp, '\n'))
		{
			count++;
		}
		fp.close();
		return count;
	}
}


int f_get_columns(const string filename)
{
	int count = 0;
	string temp;
	char flag = 0;
	fstream fp;
	fp.open(filename, ios::in);

	getline(fp, temp, '\n');

	while (temp[flag] != '\0' &&  temp[flag] != '\n')
	{
		while (temp[flag] != '\0' && temp[flag] != ' ' &&  temp[flag] != '\n')
		{
			flag++;
		}
		if (flag != 0)
		{
			count++;
		}
		while (temp[flag] != '\0' && temp[flag] == ' ' && temp[flag] != '\n')
		{
			flag++;
		}
	}

	fp.close();
	return count;
}
*/