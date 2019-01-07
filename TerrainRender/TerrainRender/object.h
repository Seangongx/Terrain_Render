/*
//������Ķ�����
*/
#pragma once
#include <glad/glad.h>
typedef struct NodeCordinate//��������Ļ���Ͻ�Ϊê��
{
	GLint cor_x;//�������������
	GLint cor_y;//������ϵ�������
	GLint cor_z;//�߶�
};


typedef struct Node
{
	NodeCordinate center;
	NodeCordinate corner_top_right;
	NodeCordinate corner_top_left;
	NodeCordinate corner_bottom_left;
	NodeCordinate corner_bottom_right;
	NodeCordinate edge_top;
	NodeCordinate edge_bottom;
	NodeCordinate edge_left;
	NodeCordinate edge_right;
}; 

class Object
{
	Node point;
public:
	Object(float _x, float _y, float _width, float _height);
	~Object();
public:
	//��������ԣ���������ͳ��������Ͻ�Ϊê��
	float x;
	float y;
	float width;
	float height;
};
