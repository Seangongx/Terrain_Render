#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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

#endif