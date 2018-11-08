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

//���ú���
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

	///��ά������ʽ�洢
	double **verticesMap;
	unsigned int **indicesMap;
	///һά�������洢
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
	///dimension ���ݸ�ʽ��һά���������ά���飩
	void Show_Vertices(int dimension);
	///dimension ���ݸ�ʽ��һά���������ά���飩
	void Show_Indices(int dimension);

};

#endif