/*
//被管理的对象类
*/
#pragma once
#include <glad/glad.h>
typedef struct NodeCordinate//坐标以屏幕左上角为锚点
{
	GLint cor_x;//坐标从左到右增加
	GLint cor_y;//坐标从上到下增加
	GLint cor_z;//高度
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
	//对象的属性，例如坐标和长宽，以左上角为锚点
	float x;
	float y;
	float width;
	float height;
};
