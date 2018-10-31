/*
//四叉树节点类，用头节点代表四叉树
//坐标系左上角为原点，左往右为x轴递增，上往下y轴递增
//本四叉树的策略是：
1，插入时动态分配节点和删除节点，不是满树；
2，当矩形区域完全包含某个节点时才获取或剔除；
3，对象放在完全包含它的区域节点内，非根节点也存储对象
*/
#pragma once
#include <list>

//四叉树类型枚举
enum QuadType
{
	ROOT,         //根		0
	UP_RIGHT,     //象限Ⅰ	1
	UP_LEFT,      //象限Ⅱ	2
	BOTTOM_LEFT,  //象限Ⅲ	3
	BOTTOM_RIGHT  //象限Ⅳ	4
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
	void InsertObject(T *object); //插入对象
	std::list<T *> GetObjectsAt(float px, float py, float w, float h); //查询对象,获得一片区域里的对象链表，此处只考虑完全包含的
	void RemoveObjectsAt(float px, float py, float w, float h); //删除对象，删除一片区域里的对象和节点，此处只考虑完全包含的

private:
	bool IsContain(float px, float py, float w, float h, T *object) const; //判断某个区域是否包含某对象 this的类型是const QaudTreeNode *const this
	bool IsContain(float px, float py, float w, float h, QuadTreeNode<T> *quadTreeNode) const; //重载，判断某个区域是否包含某个节点
private:
	std::list<T *> objects; //节点数据队列
	//父、子节点，分四个象限
	QuadTreeNode *parent;
	QuadTreeNode *upRightNode;
	QuadTreeNode *upLeftNode;
	QuadTreeNode *bottomLeftNode;
	QuadTreeNode *bottomRightNode;
	//节点类型
	QuadType quadType;
	//坐标和长宽属性，左上角为锚点
	float x;
	float y;
	float width;
	float height;

	int level; //当前深度
	int maxLevel; //最大深度
};


