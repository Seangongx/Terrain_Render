#pragma once
#ifndef __DATA_H__
#define __DATA_H__

#include <glad/glad.h>
#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <cfloat>//use min or max value of data type
#include <fstream>
#include <string>//getline
#include <cmath>
#include <algorithm>//for each
#include "file.h"
#include "function.h"

#define safe_delete(p)			{ if(p) { delete (p); (p)=0; } }
#define safe_delete_array(p)	{ if(p) { delete[] (p); (p)=0; } }
#define safe_release(p)			{ if(p) { (p)->Release(); (p)=0; } }

//���ú���
void d_show1vd(double * data, int row, int col);
void d_show2vd(double ** data, int row, int col);
void d_show1vui(unsigned int * data, int row, int col);
void d_show2vui(unsigned int ** data, int row, int col);

//���ε������ࣨ�洢��ʽ����Ϊ��ά����Ҳ����Ϊһά��������
class Terrain_Data
{
private:
	int vertices_rows;
	int vertices_columns;
	int indices_rows;
	int indices_columns;

	//��ά������ʽ�洢
	double **verticesMap;
	unsigned int **indicesMap;
	//һά�������洢
	double *verticesStream;
	unsigned int *indicesStream;

	int maxRows;
	int maxColumns;

public:
	//��ʼ���������ݣ���ǰ���ٿռ�
	Terrain_Data(const std::string verticesfile, const std::string indicesfile);
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
	//dimension ���ݸ�ʽ��һά���������ά���飩
	void Show_Vertices(int dimension);
	//dimension ���ݸ�ʽ��һά���������ά���飩
	void Show_Indices(int dimension);

	int Get_VerticesRows();
	int Get_IndicesRows();

};

//��־λ��ά������
class CBit
{
private:
	GLuint*		m_pBits;//�洢λ��־��ָ��
	GLuint		m_Row;//����
	GLuint		m_Col;//����

public:
	CBit();
	~CBit();

	//������־����
	bool Create(GLuint Row, GLuint Col);
	//���ñ�־λ		
	inline void Set(GLuint Row, GLuint Col, bool bFlog = true);	
	//��־����
	void Reset();
	//��ѯ�ñ�־
	inline bool IsTrue(GLuint Row, GLuint Col);

};
//��Ա����ʵ��...
inline void CBit::Set(GLuint R, GLuint C, bool bFlog)
{
	assert(m_pBits);

	bFlog ? m_pBits[(R*m_Col + C) / 32] |= (1 << (R*m_Col + C) % 32)
		: m_pBits[(R*m_Col + C) / 32] &= ~(1 << (R*m_Col + C) % 32);

}
inline bool CBit::IsTrue(GLuint R, GLuint C)
{
	assert(m_pBits);

	return  (m_pBits[(R*m_Col + C) / 32] & (1 << (R*m_Col + C) % 32)) != 0;
}




#endif