#pragma once
#ifndef __FILE_H__
#define __FILE_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "function.h"
#define Terrain

#ifdef Terrain
#include <glad/glad.h>
typedef int GLint;
typedef double GLdouble;
typedef unsigned int GLuint;
#endif

class CTerrainFile
{
public:
	enum EType {
		EMPTY = 0,
		VERTICES,
		INDICES,
		TEXTURE,
		LIGHT
	};								//文件分类

private:
	int				m_filerow;		//文件行数
	int				m_filecol;		//文件列数
	void*			m_pFileStream;	//文件流
	EType			m_eFileType;	//文件类型

public:
	CTerrainFile();
	CTerrainFile(const std::string filename, EType filetype);
	~CTerrainFile();
	void	LoadFile(const std::string filename, EType filetype);	//根据类型载入文件
	int		SizeofData();											//载入数据大小（与文件大小无关）
	void*	GetStream();											//获得文件流
	int		GetRow();												//获得数据行数
	int		GetCol();												//获得数据列数

private:
	void Load_1d_Vertices(const std::string filename);				//载入顶点
	void Load_1ui_Indices(const std::string filename);				//载入索引

};

inline void* CTerrainFile::GetStream()
{
	return m_pFileStream;
}

inline int CTerrainFile::GetRow()
{
	return m_filerow;
}

inline int CTerrainFile::GetCol()
{
	return m_filecol;
}


//获取有格式文件的行数
int f_get_rows(const std::string filename)
{
	int count = 0;
	std::string temp;
	std::fstream fin(filename, std::ios::in);
	if (!fin)
	{
		std::cout << "The file is not exist!" << std::endl;
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

//获取有格式文件的列数
int f_get_columns(const std::string filename)
{
	std::string rowstring;
	std::vector<std::string> temp;
	std::ifstream fin(filename, std::ios::in);

	if (!fin) {
		std::cout << "The file is not exist!" << std::endl;
		return -1;
	}
	getline(fin, rowstring, '\n');
	if (rowstring.find("\t") && rowstring.find(" ") == -1)
		temp = str_split(rowstring, "\t");
	else
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