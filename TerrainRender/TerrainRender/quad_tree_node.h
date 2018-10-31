/*
//�Ĳ����ڵ��࣬��ͷ�ڵ�����Ĳ���
//����ϵ���Ͻ�Ϊԭ�㣬������Ϊx�������������y�����
//���Ĳ����Ĳ����ǣ�
1������ʱ��̬����ڵ��ɾ���ڵ㣬����������
2��������������ȫ����ĳ���ڵ�ʱ�Ż�ȡ���޳���
3�����������ȫ������������ڵ��ڣ��Ǹ��ڵ�Ҳ�洢����
*/
#pragma once
#include <list>

//�Ĳ�������ö��
enum QuadType
{
	ROOT,         //��		0
	UP_RIGHT,     //���ޢ�	1
	UP_LEFT,      //���ޢ�	2
	BOTTOM_LEFT,  //���ޢ�	3
	BOTTOM_RIGHT  //���ޢ�	4
};

template <typename T>
class QuadTreeNode
{
public:
	QuadTreeNode(
		float _x, float _y, float _width, float _height, 
		int _level, int _maxLevel,
		QuadType _quadType, QuadTreeNode *_parent);
	~QuadTreeNode();
public:
	void InsertObject(T *object); //�������
	std::list<T *> GetObjectsAt(float px, float py, float w, float h); //��ѯ����,���һƬ������Ķ��������˴�ֻ������ȫ������
	void RemoveObjectsAt(float px, float py, float w, float h); //ɾ������ɾ��һƬ������Ķ���ͽڵ㣬�˴�ֻ������ȫ������

private:
	bool IsContain(float px, float py, float w, float h, T *object) const; //�ж�ĳ�������Ƿ����ĳ���� this��������const QaudTreeNode *const this
	bool IsContain(float px, float py, float w, float h, QuadTreeNode<T> *quadTreeNode) const; //���أ��ж�ĳ�������Ƿ����ĳ���ڵ�
private:
	std::list<T *> objects; //�ڵ����ݶ���
	//�����ӽڵ㣬���ĸ�����
	QuadTreeNode *parent;
	QuadTreeNode *upRightNode;
	QuadTreeNode *upLeftNode;
	QuadTreeNode *bottomLeftNode;
	QuadTreeNode *bottomRightNode;
	//�ڵ�����
	QuadType quadType;
	//����ͳ������ԣ����Ͻ�Ϊê��
	float x;
	float y;
	float width;
	float height;

	int level; //��ǰ���
	int maxLevel; //������
};


