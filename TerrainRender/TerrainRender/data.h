#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <cstdlib>
#include <cfloat>//use min or max value of data type
#include <fstream>
#include <string>//getline
#include <cmath>
#include <vector>
#include <algorithm>//for each
using namespace std;

#define MAXDATA 10000000//Max data quantity
#define MAXROWS 32768//Initial value of rows (2pow(7+7+1)) 
#define MAXCOLUMNS 4//Initial value of coloumns
#define READCHAR 30
#define READROW 200

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

class Terrain_Data
{
private:
	int vertices_rows;
	int vertices_columns;
	int indices_rows;
	int indices_columns;

	///二维数组形式存储
	double **verticesMap = NULL;
	unsigned int **indicesMap = NULL;
	///一维数据流存储
	double *verticesStream = NULL;
	unsigned int *indicesStream = NULL;

	int maxRows = MAXROWS;
	int maxColumns = MAXCOLUMNS;

public:
	Terrain_Data(int vrows, int vcols, int irows, int icols)
	{
		vertices_rows = vrows; vertices_columns = vcols;
		indices_rows = irows; indices_columns = icols;
		verticesStream = new double[vertices_rows * vertices_columns * sizeof(double)]();
		if (verticesStream == NULL)
			cout << "顶点数据内存分配失败！" << endl;
		indicesStream = new unsigned int[indices_rows * indices_columns * sizeof(unsigned int)]();
		if (indicesStream == NULL)
			cout << "索引数据内存分配失败！" << endl;

		/*c动态开辟写法
		verticesMap = (double**)malloc(vertices_rows * sizeof(double*));//注意sizeof的double*
		indicesMap = (unsigned int**)malloc(indices_rows * sizeof(unsigned int*));

		int i = 0, j = 0;
		for (i = 0; i < vertices_rows; i++)
		{
			verticesMap[i] = (double*)malloc(vertices_columns * sizeof(double));
			for (j = 0; j < vertices_columns; j++)
			{
				verticesMap[i][j] = DBL_MIN;
			}
		}
		for (i = 0; i < indices_rows; i++)
		{
			indicesMap[i] = (unsigned int*)malloc(indices_columns * sizeof(unsigned int));
			for (j = 0; j < indices_columns; j++)
			{
				indicesMap[i][j] = INT_MIN;
			}
		}*/

	}
	
	~Terrain_Data()
	{
		if (Data_State() == 1)
		{
			Clear_Data();
		}
	}

	int Data_State()
	{
		/*
		if (verticesMap[0][0] == DBL_MIN || indicesMap[0][0] == INT_MIN)
		{
			cout << "data empty!" << endl;
			return;
		}
		*/

		if (verticesMap == NULL || indicesMap == NULL)
		{
			cout << "data empty!" << endl;
			return 0;
		}
		else
		{
			cout << "data loaded!" << endl;
			return 1;
		}
	}

	void Clear_Data()
	{
		/*
		for (int i = 0; i < vertices_rows; i++)
		{
			delete[] verticesMap[i];
		}
		delete[] verticesMap;
		verticesMap = NULL;

		for (int i = 0; i < indices_rows; i++)
		{
			delete[] indicesMap[i];
		}
		delete[] indicesMap;
		indicesMap = NULL;
		*/

		delete[] verticesStream; verticesStream = NULL;
		delete[] indicesStream; indicesStream = NULL;
	}

	double** Load_Vertices(const string filename)
	{
		char num[READCHAR];//use atol
		//char rowstring[READROW];
		string rowstring;
		int rowcount = 0;
		int coloumncount = 0;
		int flag = 0;//whole string count
		int i = 0;//substring count

		ifstream fin(filename, ios::in);
		if (!fin) {
			cout << "The file is not exist!" << endl;
			return NULL;
		}
		while (getline(fin, rowstring))
		{
			while (rowstring[flag] != '\0' &&  rowstring[flag] != '\n')
			{
				while (rowstring[flag] != '\0' && rowstring[flag] != ' ' &&  rowstring[flag] != '\n')
				{
					num[i] = rowstring[flag];
					i++;
					flag++;
				}
				num[i] = '\0';
				i = 0;
				verticesMap[rowcount][coloumncount] = atol(num);
				coloumncount++;
				while (rowstring[flag] != '\0' && rowstring[flag] == ' ' && rowstring[flag] != '\n')
				{
					flag++;
				}
			}
			rowcount++;
			coloumncount = 0;
			flag = 0;
		}

		fin.close();
		return verticesMap;
	}

	double* Load_1d_Vertices(const string filename)
	{
		string rowstring;
		vector<string> temp;
		int count = 0;

		ifstream fin(filename, ios::in);
		if (!fin) {
			cout << "The file is not exist!" << endl;
			return NULL;
		}
		while (getline(fin, rowstring))
		{
			temp = str_split(rowstring, " ");
			for (int i = 0; i < temp.size()-1; i++)
			{
				verticesStream[count++] = stod(temp[i]);
			}
			temp.clear();
		}

		fin.close();
		return verticesStream;

	}

	unsigned int** Load_Indices(const string filename)
	{
		char num[READCHAR];
		//char rowstring[READROW];
		string rowstring;
		int rowcount = 0;
		int coloumncount = 0;
		int flag = 0;//whole string count
		int i = 0;//substring count

		ifstream fin(filename, ios::in);
		if (!fin) {
			cout << "The file is not exist!" << endl;
			return NULL;
		}
		while (getline(fin, rowstring))
		{
			while (rowstring[flag] != '\0' &&  rowstring[flag] != '\n')
			{
				while (rowstring[flag] != '\0' && rowstring[flag] != ' ' &&  rowstring[flag] != '\n')
				{
					num[i] = rowstring[flag];
					i++;
					flag++;
				}
				num[i] = '\0';
				i = 0;
				indicesMap[rowcount][coloumncount] = atoi(num);
				coloumncount++;
				while (rowstring[flag] != '\0' && rowstring[flag] == ' ' && rowstring[flag] != '\n')
				{
					flag++;
				}
			}
			rowcount++;
			coloumncount = 0;
			flag = 0;
		}

		fin.close();
		return indicesMap;
	}

	unsigned int * Load_1ui_Indices(const string filename)
	{
		string rowstring;
		vector<string> temp;
		int count = 0;

		ifstream fin(filename, ios::in);
		if (!fin) {
			cout << "The file is not exist!" << endl;
			return NULL;
		}
		while (getline(fin, rowstring))
		{
			temp = str_split(rowstring, " ");
			for (int i = 0; i < temp.size()-1; i++)
			{
				indicesStream[count++] = stod(temp[i]);
			}
			temp.clear();
		}

		fin.close();
		return indicesStream;
	}

	int Sizeof_Vertices()//TODO:var types
	{
		//all of pointer space are 4bytes
		return vertices_rows * 3 * sizeof(double);
	}

	int Sizeof_Indices()//TODO:var types
	{
		//all of pointer space are 4bytes
		return indices_rows * 3 * sizeof(unsigned);
	}

	void Show_Vertices(int dimension)
	{
		if (verticesMap == NULL)
		{
			cout << "data empty!" << endl;
			return;
		}
		/*
		if (verticesMap[0][0] == DBL_MIN)
		{
			cout << "data empty!" << endl;
			return;
		}
		*/
		if (dimension == 1)
		{
			d_show1vd(verticesStream, vertices_rows, 3);
		}
		else if (dimension == 2)
		{
			d_show2vd(verticesMap, vertices_rows, 3);
		}
		return;
	}

	void Show_Indices(int dimension)
	{
		if (indicesMap == NULL)
		{
			cout << "data empty!" << endl;
			return;
		}
		/*
		if (indicesMap[0][0] == INT_MIN)
		{
			cout << "data empty!" << endl;
			return;
		}
		*/
		if (dimension == 1)
		{
			d_show1vui(indicesStream, indices_rows, 3);
		}
		else if (dimension == 2)
		{
			d_show2vui(indicesMap, indices_rows, 3);
		}
		return;
	}

};


void d_show1vd(double * data, int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		cout << i << ":";
		for (j = 0; j < col; j++)
		{
			cout << data[i * 3 + j] << '\t';
		}
		cout << endl;
	}
	cout << endl;
}

void d_show2vd(double ** data, int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		cout << i << ":";
		for (j = 0; j < col; j++)
		{
			cout << data[i][j] << '\t';
		}
		cout << endl;
	}
	cout << endl;
}

void d_show1vui(unsigned int * data, int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		cout << i << ":";
		for (j = 0; j < col; j++)
		{
			cout << data[i * 3 + j] << '\t';
		}
		cout << endl;
	}
	cout << endl;
}

void d_show2vui(unsigned int ** data, int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		cout << i << ":";
		for (j = 0; j < col; j++)
		{
			cout << data[i][j] << '\t';
		}
		cout << endl;
	}
	cout << endl;
}

bool d_pick_vertices(double** vertices, double * pick, int rows, int cols)
{
	int i = 0, j = 0, count = 0;
	//int rows = MAXROWS / 2;
	//int cols = MAXCOLUMNS - 1;//一般为3
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
				pick[count++] = vertices[i][j];
		}
	}
	if (count == rows*cols)
		return true;
	else return false;
}

bool d_pick_indices(unsigned int** indices, unsigned * pick, int rows, int cols)
{
	int i = 0, j = 0, count = 0;
	//int rows = pow((sqrt(MAXROWS / 2) - 1), 2) * 2;
	//int cols = MAXCOLUMNS - 1;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			pick[count++] = indices[i][j];
		}
	}	
	if (count == rows*cols)
		return true;
	else return false;
}

bool d_copy_vertices(double* vertices, double* copy, int count)
{
	int i = 0;
	for (; i < count; i++)
		copy[i] = vertices[i];
	if (i == count)
		return true;
	else return false;
}

bool d_copy_indices(unsigned int* indices, unsigned int* copy, int count)
{
	int i = 0;
	for (; i < count; i++)
		copy[i] = indices[i];
	if (i == count)
		return true;
	else return false;
}



#endif