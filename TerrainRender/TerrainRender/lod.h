#pragma once
#include <vector>
#include "data.h"

class CLOD
{
public:
	virtual void Build() = 0;				//������β��ϸ��
	virtual void Init() = 0;				//��ʼ�����β��ϸ��
	virtual GLint BuildTrianlges() = 0;		//����ʵ����Ⱦ������ƬԪ����
	virtual void SetFactor(float f) = 0;	//���δֲ�����
	virtual ~CLOD(){}

};


//����4������LOD
class CLOD_Quadtree : public CLOD
{
private:
	struct NODE
	{
		int x, y;

		NODE(int _x, int _y) :x(_x), y(_y) {};
		NODE() {};
	};
	enum { AD_LEFT = 0, AD_RIGHT, AD_UP, AD_DOWN };
	GLuint					m_nLevel;			//�ȼ�(�ֱ���)
	GLuint					m_cur_Level;		//��ǰ�ȼ�
	GLuint					m_AdjctAct[4];		//���ڽڵ��Ƿ��Ծ
	CBit					m_Bit;				//��ǽڵ�
	GLfloat					m_factor;			//�ֲ�����
	GLfloat*				m_pVariant;			//�ر�ֲ��α�
	CTerrain*				m_data;				//��������
	CCamera*				m_Cam;				//�����
	GLfloat					m_cellSize;			//��λ����
	std::vector<GLint>		m_RenderTable;		//������Ⱦ��
	IDirect3DIndexBuffer9*	m_pIB;				//�ڵ��������棨���Σ�
	GLint					m_BuildTrianlges;	//����������Ŀ

	//˽�г�Ա����...�ڲ�ʹ��
	GLfloat InitVariant(GLint level, GLint x, GLint y);	//��ʼ���ڵ����ֵ
	void AttachNode(const NODE & node);					//���Ͻڵ�
	bool NodeIsVisible(const NODE & node);				//�ڵ�ɼ�
	bool NodeCanDivid(const NODE & node);				//�ڵ��ָܷ�
	void DisableNode(const NODE & node);				//ʹ�˽ڵ���Ч
	void DividNode(const NODE & node);					//��ִ˽ڵ�


public:
	CLOD_Quadtree();
	~CLOD_Quadrtee();
	void Init(CTerrain * T, CCamera* C);					//��ʼ�����β��ϸ��
	void Build();											//������ε�LOD
	GLint BuildTrianlges() { return m_BuildTrianlges; }
	void SetFactor(GLfloat f) { m_factor = f; }

};