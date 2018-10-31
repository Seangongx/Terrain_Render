#include "quad_tree_node.h"

template <typename T>
QuadTreeNode<T>::QuadTreeNode(
	float _x, float _y, float _width, float _height,
	int _level, int _maxLevel,
	QuadType _quadType,
	QuadTreeNode *_parent) :
	x(_x),
	y(_y),
	width(_width),
	height(_height),
	level(_level),
	maxLevel(_maxLevel),
	quadType(_quadType)//冒号之后的声明为构造函数初始化列表
{
	parent = _parent;
	upLeftNode = nullptr;
	upRightNode = nullptr;
	bottomLeftNode = nullptr;
	bottomRightNode = nullptr;
}

template <typename T>
QuadTreeNode<T>::~QuadTreeNode()
{
	if (level == maxLevel)
		return;
	//如果不是叶子节点，就销毁子节点
	parent = nullptr;

}

template <typename T>
bool QuadTreeNode<T>::IsContain(float px, float py, float w, float h, T *object) const
{
	if (object->x >= px
		&& object->x + object->width <= px + w
		&& object->y >= py
		&& object->y + object->height <= py + h)
		return true;
	return false;
}

template <typename T>
bool QuadTreeNode<T>::IsContain(float px, float py, float w, float h, QuadTreeNode<T> *quadTreeNode) const
{
	if (quadTreeNode->x >= px
		&& quadTreeNode->x + quadTreeNode->width <= px + w
		&& quadTreeNode->y >= py
		&& quadTreeNode->y + quadTreeNode->height <= py + h)
		return true;
	return false;
}

template <typename T>
void QuadTreeNode<T>::InsertObject(T *object)
{
	//如果是叶子节点，则存在叶子节点
	if (level == maxLevel)
	{
		objects.push_back(object);
		return;
	}

	//非叶子节点，如果下层节点可以包含该对象，则递归构建子节点并插入对象,边构建边插入
	if (IsContain(x + width / 2, y, width / 2, height / 2, object))
	{
		if (!upRightNode) //避免重复创建覆盖掉原来的节点
			upRightNode = new QuadTreeNode(x + width / 2, y, width / 2, height / 2, level + 1, maxLevel, UP_RIGHT, this);//如果没有子节点就创建子节点，parent节点是当前节点
		upRightNode->InsertObject(object);
		return;
	}
	else if (IsContain(x, y, width / 2, height / 2, object))
	{
		if (!upLeftNode)
			upLeftNode = new QuadTreeNode(x, y, width / 2, height / 2, level + 1, maxLevel, UP_LEFT, this);
		upLeftNode->InsertObject(object);
		return;
	}
	else if (IsContain(x, y + height / 2, width / 2, height / 2, object))
	{
		if (!bottomLeftNode)
			bottomLeftNode = new QuadTreeNode(x, y + height / 2, width / 2, height / 2, level + 1, maxLevel, BOTTOM_LEFT, this);
		bottomLeftNode->InsertObject(object);
		return;
	}
	else if (IsContain(x + width / 2, y + height / 2, width / 2, height / 2, object))
	{
		if (!bottomRightNode)
			bottomRightNode = new QuadTreeNode(x + width / 2, y + height / 2, width / 2, height / 2, level + 1, maxLevel, BOTTOM_RIGHT, this);
		bottomRightNode->InsertObject(object);
		return;
	}
	//下层节点不能完全包含改对象，则插入到当前非叶子节点
	//这个判断也可以省去
	if (IsContain(x, y, width, height, object))
		objects.push_back(object);
}

template <typename T>
std::list<T *> QuadTreeNode<T>::GetObjectsAt(float px, float py, float w, float h)
{
	std::list<T *> resObjects;
	//如果当前节点完全被包含，把当前节点存的对象放到列表末尾,空链表也行
	if (IsContain(px, py, w, h, this))
	{
		resObjects.insert(resObjects.end(), objects.begin(), objects.end());
		//最后一层
		if (level == maxLevel)
			return resObjects;
	}

	//如果有下层节点就把下层节点包含的对象加进来
	if (upRightNode)
	{
		std::list<T *> upRightChild;
		upRightChild = upRightNode->GetObjectsAt(px, py, w, h);
		resObjects.insert(resObjects.end(), upRightChild.begin(), upRightChild.end());
	}
	if (upLeftNode)
	{
		std::list<T *> upLeftChild;
		upLeftChild = upLeftNode->GetObjectsAt(px, py, w, h);
		resObjects.insert(resObjects.end(), upLeftChild.begin(), upLeftChild.end());
	}
	if (bottomLeftNode)
	{
		std::list<T *> bottomLeftChild;
		bottomLeftChild = bottomLeftNode->GetObjectsAt(px, py, w, h);
		resObjects.insert(resObjects.end(), bottomLeftChild.begin(), bottomLeftChild.end());
	}
	if (bottomRightNode)
	{
		std::list<T *> bottomRightChild;
		bottomRightChild = bottomRightNode->GetObjectsAt(px, py, w, h);
		resObjects.insert(resObjects.end(), bottomRightChild.begin(), bottomRightChild.end());
	}
	return resObjects;
}

template <typename T>
void QuadTreeNode<T>::RemoveObjectsAt(float px, float py, float w, float h)
{
	//如果本层节点被包含则删除本层节点的对象
	//这个判断主要是对根节点起作用，其他子节点实际在上层都做了判断
	if (IsContain(px, py, w, h, this))
	{
		//清除本节点层的对象
		objects.clear();
		//最后一层
		if (level == maxLevel)
			return;

	}
	//如果有子节点且被包含就销毁子节点，注意别产生野指针
	//其实只要上层被包含了，下层肯定被包含，代码还需改进
	if (upRightNode&&IsContain(px, py, w, h, upRightNode))
	{
		upRightNode->RemoveObjectsAt(px, py, w, h);
		delete upRightNode;
		upRightNode = nullptr;

	}
	if (upLeftNode&&IsContain(px, py, w, h, upLeftNode))
	{
		upLeftNode->RemoveObjectsAt(px, py, w, h);
		delete upLeftNode;
		upLeftNode = nullptr;

	}
	if (bottomLeftNode&&IsContain(px, py, w, h, bottomLeftNode))
	{
		bottomLeftNode->RemoveObjectsAt(px, py, w, h);
		delete bottomLeftNode;
		bottomLeftNode = nullptr;

	}
	if (bottomRightNode&&IsContain(px, py, w, h, bottomRightNode))
	{
		bottomRightNode->RemoveObjectsAt(px, py, w, h);
		delete bottomRightNode;
		bottomRightNode = nullptr;
	}
}