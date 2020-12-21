#pragma once
#ifndef __DATA_H__
#define __DATA_H__

//#include <glad/glad.h> file.h���Ѱ���
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

#include <glm/glm.hpp>//����ͼ�μ����
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define safe_delete(p)			{ if(p) { delete (p); (p)=0; } }
#define safe_delete_array(p)	{ if(p) { delete[] (p); (p)=0; } }
#define safe_release(p)			{ if(p) { (p)->Release(); (p)=0; } }


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


class CLod;
//CLOD����������
class CTerrain
{
private:
	//TODO:���ηֿ�
	struct SVertex
	{
		GLfloat		height;//�߶�
		//glm::vec3	Normal;//������
	};

	friend class CLOD_Quadtree;

	CTerrainFile		m_vertices;			//�����ļ�����
	CTerrainFile		m_indices;			//�����ļ�����
	CLod*				m_lod;				//���ϸ������
	GLdouble*			m_pVerticesStream;	//����������
	GLuint*				m_pIndicesStream;	//����������
	GLdouble*			m_pVB;				//���㻺��
//	GLuint*				m_pEB;				//��������
	GLuint				m_size;				//��ͼ��С�����=�߶ȣ�
	GLuint				m_VAO, m_VBO, m_EBO;

	bool CreateBuffer();								//�������������������

public:
	CTerrain();
	CTerrain(const std::string verticesfile);
	CTerrain(const std::string verticesfile, const std::string indicesfile);
	~CTerrain();

	bool Create(GLuint size);							//������ͼ
	void Render();										//��Ⱦ����
	void SetLodType(CLod* lod);							//����LOD���ͣ�Quad\Roam...)
	void Delete();										//ɾ���������
	GLuint GetSize();									//���ص�ͼ��С
	GLdouble GetHeight(GLint _x, GLint _y);				//����ĳһ����ĸ߶�ֵ���������ȣ�
	GLdouble GetHeight_P2C(GLint _x, GLint _y);			//����ĳһ����ĸ߶�ֵ���������ȣ�
	glm::vec3 GetPos(GLint _x, GLint _y);				//����ĳһ�������ʵλ��
	GLuint* GetVAO();									//���ض����������
	GLuint* GetVBO();									//���ض��㻺�����
	GLint	GetVBOSize();								//���ض��������С
	bool GenerateObject();								//�����������
};


inline GLuint* CTerrain::GetVAO()
{
	return &m_VAO;
}
inline GLuint* CTerrain::GetVBO()
{
	return &m_VBO;
}
inline GLint CTerrain::GetVBOSize()
{
	return m_vertices.SizeofData();
}



#endif

#ifdef OLD

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

#endif

