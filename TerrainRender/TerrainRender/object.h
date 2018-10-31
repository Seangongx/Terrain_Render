/*
//被管理的对象类
*/
#pragma once
class Object
{
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
