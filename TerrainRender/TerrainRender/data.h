#pragma once
#ifndef __DATA_H__
#define __DATA_H__

#include <iostream>
#include <cstdlib>
#include <cfloat>//use min or max value of data type
#include <fstream>
#include <string>//getline
#include <cmath>
#include <algorithm>//for each
#include "function.h"

//公用函数
void d_show1vd(double * data, int row, int col);
void d_show2vd(double ** data, int row, int col);
void d_show1vui(unsigned int * data, int row, int col);
void d_show2vui(unsigned int ** data, int row, int col);

class Terrain_Data
{
private:
	int vertices_rows;
	int vertices_columns;
	int indices_rows;
	int indices_columns;

	///二维数组形式存储
	double **verticesMap;
	unsigned int **indicesMap;
	///一维数据流存储
	double *verticesStream;
	unsigned int *indicesStream;

	int maxRows;
	int maxColumns;

public:
	Terrain_Data(int vrows, int vcols, int irows, int icols);
	~Terrain_Data();
	int Data_State();
	void Clear_Data();

	double** Load_Vertices(const std::string filename);
	double* Load_1d_Vertices(const std::string filename);
	unsigned int** Load_Indices(const std::string filename);
	unsigned int * Load_1ui_Indices(const std::string filename);

	int Sizeof_Vertices();
	int Sizeof_Indices();
	///dimension 数据格式（一维数据流或二维数组）
	void Show_Vertices(int dimension);
	///dimension 数据格式（一维数据流或二维数组）
	void Show_Indices(int dimension);

};

#endif