#pragma once
#ifndef __DATA_H__
#define __DATA_H__

//#include <glad/glad.h> file.h内已包含
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

#include <glm/glm.hpp>//引入图形计算库
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define safe_delete(p)			{ if(p) { delete (p); (p)=0; } }
#define safe_delete_array(p)	{ if(p) { delete[] (p); (p)=0; } }
#define safe_release(p)			{ if(p) { (p)->Release(); (p)=0; } }


//标志位二维数组类
class CBit
{
private:
	GLuint*		m_pBits;//存储位标志的指针
	GLuint		m_Row;//行数
	GLuint		m_Col;//列数

public:
	CBit();
	~CBit();

	//创建标志数组
	bool Create(GLuint Row, GLuint Col);
	//设置标志位		
	inline void Set(GLuint Row, GLuint Col, bool bFlog = true);	
	//标志清零
	void Reset();
	//查询该标志
	inline bool IsTrue(GLuint Row, GLuint Col);

};
//成员函数实现...
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
//CLOD地形数据类
class CTerrain
{
private:
	//TODO:地形分块
	struct SVertex
	{
		GLfloat		height;//高度
		//glm::vec3	Normal;//法向量
	};

	friend class CLOD_Quadtree;

	CTerrainFile		m_vertices;			//顶点文件操作
	CTerrainFile		m_indices;			//索引文件操作
	CLod*				m_lod;				//层次细节类型
	GLdouble*			m_pVerticesStream;	//顶点数据流
	GLuint*				m_pIndicesStream;	//索引数据流
	GLdouble*			m_pVB;				//顶点缓存
//	GLuint*				m_pEB;				//索引缓存
	GLuint				m_size;				//地图大小（宽度=高度）
	GLuint				m_VAO, m_VBO, m_EBO;

	bool CreateBuffer();								//创建共享顶点和索引缓存

public:
	CTerrain();
	CTerrain(const std::string verticesfile);
	CTerrain(const std::string verticesfile, const std::string indicesfile);
	~CTerrain();

	bool Create(GLuint size);							//创建地图
	void Render();										//渲染地形
	void SetLodType(CLod* lod);							//设置LOD类型（Quad\Roam...)
	void Delete();										//删除缓存对象
	GLuint GetSize();									//返回地图大小
	GLdouble GetHeight(GLint _x, GLint _y);				//返回某一坐标的高度值（按行优先）
	GLdouble GetHeight_P2C(GLint _x, GLint _y);			//返回某一坐标的高度值（按列优先）
	glm::vec3 GetPos(GLint _x, GLint _y);				//返回某一坐标的真实位置
	GLuint* GetVAO();									//返回顶点数组对象
	GLuint* GetVBO();									//返回顶点缓冲对象
	GLint	GetVBOSize();								//返回顶点数组大小
	bool GenerateObject();								//产生顶点对象
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

//公用函数
void d_show1vd(double * data, int row, int col);
void d_show2vd(double ** data, int row, int col);
void d_show1vui(unsigned int * data, int row, int col);
void d_show2vui(unsigned int ** data, int row, int col);

//地形的数据类（存储方式可以为二维数组也可以为一维数据流）
class Terrain_Data
{
private:
	int vertices_rows;
	int vertices_columns;
	int indices_rows;
	int indices_columns;

	//二维数组形式存储
	double **verticesMap;
	unsigned int **indicesMap;
	//一维数据流存储
	double *verticesStream;
	unsigned int *indicesStream;

	int maxRows;
	int maxColumns;

public:
	//初始化地形数据，提前开辟空间
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
	//dimension 数据格式（一维数据流或二维数组）
	void Show_Vertices(int dimension);
	//dimension 数据格式（一维数据流或二维数组）
	void Show_Indices(int dimension);

	int Get_VerticesRows();
	int Get_IndicesRows();

};

#endif

