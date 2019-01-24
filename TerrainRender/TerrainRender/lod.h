#pragma once
#ifndef __LOD_H__
#define __LOD_H__

#include <vector>
#include "data.h"
#include "camera.h"

//LOD抽象类
class CLod
{
public:
	virtual void Build() = 0;				//计算地形层次细节
	virtual void Init() = 0;				//初始化地形层次细节
	virtual GLint BuildTrianlges() = 0;		//地形实际渲染三角形片元总数
	virtual void SetFactor(float f) = 0;	//地形粗糙因子
	virtual ~CLod(){}

};

//基于4叉树的LOD
class CLod_Quadtree : public CLod
{
private:
	struct NODE
	{
		int x, y;
		NODE(int _x, int _y) :x(_x), y(_y) {};
		NODE() {};
	};

	enum { 
		AD_LEFT = 0,
		AD_RIGHT,
		AD_UP,
		AD_DOWN 
	};

	GLuint					m_nLevel;			//等级(分辨率)等级越高分辨率越小
	GLuint					m_cur_Level;		//当前等级
	GLuint					m_AdjctAct[4];		//相邻节点是否活跃
	CBit					m_Bit;				//标记节点
	GLfloat					m_factor;			//粗糙因子
	GLfloat*				m_pVariant;			//地表粗糙形变
	CTerrain*				m_data;				//地形数据
	CCamera*				m_Cam;				//摄像机
	GLfloat					m_cellSize;			//单位距离
	std::vector<GLint>		m_RenderTable;		//地形渲染表
	//IDirect3DIndexBuffer9*	m_pIB;			//节点索引缓存（扇形）
	GLint					m_BuildTrianlges;	//三角形总数目

	//私有成员方法...内部使用
	GLfloat InitVariant(GLint level, GLint x, GLint y);		//初始化节点误差值
	GLfloat InitVariant_P2C(GLint level, GLint x, GLint y);	//初始化节点误差值（数据案列优先）
	void AttachNode(const NODE & node);						//挂上节点
	bool NodeIsVisible(const NODE & node);					//节点可见
	bool NodeCanDivid(const NODE & node);					//节点能分割
	void DisableNode(const NODE & node);					//使此节点无效
	void DividNode(const NODE & node);						//拆分此节点


public:
	CLod_Quadtree();
	~CLod_Quadtree();
	void Init(CTerrain * T, CCamera* C);					//初始化地形层次细节
	void Init_P2C(CTerrain * T, CCamera* C);				//初始化地形层次细节（数据为按列优先）
	void Build();											//计算地形的LOD
	GLint BuildTrianlges() { return m_BuildTrianlges; }		//构造的三角形数
	void SetFactor(GLfloat f) { m_factor = f; }				//设置条件因子

};

#endif