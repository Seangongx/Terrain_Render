#pragma once
#ifndef __LOD_H__
#define __LOD_H__

#include <vector>
#include "data.h"
#include "camera.h"

#define IB_BATCH_SIZE 10
#define IB_SIZE (IB_BATCH_SIZE*1000)

//LOD������
//ע�ⲻ����ָ��ָ�������������͵����⣨���г����ຯ������ʵ�֣�
class CLod
{
public:
	virtual void Build() = 0;							//������β��ϸ��
	virtual void Init(CTerrain* T, CCamera* C) = 0;		//��ʼ�����β��ϸ��
	virtual GLint BuildTrianlges() = 0;					//����ʵ����Ⱦ������ƬԪ����
	virtual void SetFactor(GLfloat f) = 0;				//���δֲ�����
	virtual ~CLod(){}

};

//����4������LOD
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

	GLuint					m_nLevel;			//�ȼ�(�ֱ���)�ȼ�Խ�߷ֱ���ԽС
	GLuint					m_cur_Level;		//��ǰ�ȼ�
	GLuint					m_AdjctAct[4];		//���ڽڵ��Ƿ��Ծ
	CBit					m_Bit;				//��ǽڵ�
	GLfloat					m_factor;			//�ֲ�����
	GLfloat*				m_pVariant;			//�ر�ֲ��α�
	CTerrain*				m_data;				//��������
	CCamera*				m_Cam;				//�����
	GLfloat					m_cellSize;			//��λ����
	std::vector<GLint>		m_RenderTable;		//������Ⱦ��
	GLuint*					m_pIB;				//�ڵ��������棨���Σ�
	GLuint					m_IBO;				//�����������
	GLint					m_BuildTrianlges;	//����������Ŀ

	//˽�г�Ա����...�ڲ�ʹ��
	GLfloat InitVariant(GLint level, GLint x, GLint y);		//��ʼ���ڵ����ֵ�����ݰ������ȣ�
	GLfloat InitVariant_P2C(GLint level, GLint x, GLint y);	//��ʼ���ڵ����ֵ�����ݰ������ȣ�
	void AttachNode(const NODE & node);						//���Ͻڵ�
	bool NodeIsVisible(const NODE & node);					//�ڵ�ɼ�
	bool NodeCanDivid(const NODE & node);					//�ڵ��ָܷ�
	void DisableNode(const NODE & node);					//ʹ�˽ڵ���Ч
	void DividNode(const NODE & node);						//��ִ˽ڵ�
	void CreatePIB(GLint size);								//��������
	void AttachPIB(GLuint** point, GLint offset);		//��������
	bool UnlockPIB(GLuint** point, GLint offset);		//�������
	void DeletePIB(const GLuint* point);					//�������


public:
	CLod_Quadtree();
	~CLod_Quadtree();
	void Init(CTerrain * T, CCamera* C);					//��ʼ�����β��ϸ��
	void Init_P2C(CTerrain * T, CCamera* C);				//��ʼ�����β��ϸ�ڣ�����Ϊ�������ȣ�
	void Build();											//������ε�LOD
	GLint BuildTrianlges() { return m_BuildTrianlges; }		//�������������
	void SetFactor(GLfloat f) { m_factor = f; }				//������������

	void Render(GLuint offset, GLint triangles);			//��Ⱦ

};

#endif