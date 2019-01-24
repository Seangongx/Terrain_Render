#include <vector>
#include "lod.h"

CLod_Quadtree::CLod_Quadtree()
{
	m_nLevel		 = 0;
	m_data			 = NULL;
	m_pVariant		 = NULL;
	m_Cam			 = NULL;
	//m_pIB			 = NULL;
	m_factor		 = 0.001f;
	m_cellSize		 = 0;
	m_BuildTrianlges = 0;
	memset(m_AdjctAct, 0, sizeof(m_AdjctAct));
}

CLod_Quadtree::~CLod_Quadtree()
{
	m_nLevel		 = 0;
	m_cur_Level		 = 0;
	m_data			 = NULL;
	m_Cam			 = NULL;
	safe_delete_array(m_pVariant);
	//safe_release(m_pIB);
}


//void CLod_Quadtree::Init(CTerrain * T, CCamera* C)
//{
//	m_data = T;
//	m_Cam = C;
//	m_Bit.Create(T->GetSize(), T->GetSize());
//
//	safe_delete_array(m_pVariant);
//
//	GLuint input = T->GetSize();
//	register GLint All_Level;//注意此处有没有问题
//	_asm bsr eax, input
//	_asm mov All_Level, eax
//
//	m_nLevel = All_Level;
//	int size = 1 << (All_Level - 1);
//	m_pVariant = new GLfloat[T->GetSize() * T->GetSize()];
//
//	//m_RenderTable.reserve(T->m_SectionCountSq * T->m_SectionCountSq);
//
//	InitVariant(All_Level, size, size);
//
//	//注意getpos函数把数据x作为x横坐标,z作为y的纵坐标
//	GLfloat cellX = m_data->GetPos(1, 0).x - m_data->GetPos(0, 0).x;
//	GLfloat cellY = m_data->GetPos(0, 1).z - m_data->GetPos(0, 0).z;
//	cellX > cellY ? m_cellSize = cellX : m_cellSize = cellY;
//
//	//TODO:
//	//数据m_data创建索引缓存数组
//
//}

void CLod_Quadtree::Init_P2C(CTerrain * T, CCamera* C)
{
	m_data = T;
	m_Cam = C;
	m_Bit.Create(T->GetSize(), T->GetSize());

	safe_delete_array(m_pVariant);

	GLuint input = T->GetSize();
	register GLint All_Level;//注意此处有没有问题
	_asm bsr eax, input
	_asm mov All_Level, eax

	m_nLevel = All_Level;
	int size = 1 << (All_Level - 1);
	m_pVariant = new GLfloat[T->GetSize() * T->GetSize()];

	InitVariant(All_Level, size, size);

	GLfloat cellX = m_data->GetPos(1, 0).x - m_data->GetPos(0, 0).x;
	GLfloat cellY = m_data->GetPos(0, 1).y - m_data->GetPos(0, 0).y;
	cellX > cellY ? m_cellSize = cellX : m_cellSize = cellY;

	//TODO:
	//数据m_data创建索引缓存数组
	

}

GLfloat CLod_Quadtree::InitVariant(GLint level, GLint x, GLint y)
{
	assert(m_data);
	assert(x >= 0 && y >= 0);
	assert(x < (GLint)m_data->GetSize() && y < (GLint)m_data->GetSize());

	GLfloat Var10[10] = { 0 };
	GLint iter = 0;

	GLint index = x * m_data->GetSize() + y;//计算在缓存中的位置

	if (level > 1)
	{
		GLuint Size = 1 << (level - 2);

		Var10[iter++] = InitVariant(level - 1, x + Size, y + Size);
		Var10[iter++] = InitVariant(level - 1, x - Size, y - Size);
		Var10[iter++] = InitVariant(level - 1, x + Size, y - Size);
		Var10[iter++] = InitVariant(level - 1, x - Size, y + Size);
	}

	GLuint Size = 1 << (level - 1);
	GLfloat lu = m_data->GetHeight(x - Size, y + Size);
	GLfloat ld = m_data->GetHeight(x - Size, y - Size);
	GLfloat ru = m_data->GetHeight(x + Size, y + Size);
	GLfloat rd = m_data->GetHeight(x + Size, y - Size);

	GLfloat left = (lu + ld) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight(x - Size, y) - left);

	GLfloat right = (ru + rd) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight(x + Size, y) - right);

	GLfloat up = (ru + lu) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight(x, y + Size) - up);

	GLfloat down = (rd + ld) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight(x, y - Size) - down);

	GLfloat center = (lu + rd) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight(x, y) - center);

	center = (ld + ru) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight(x, y) - center);

	if (level > 1)
		assert(10 == iter);
	else 
		assert(6 == iter);

	GLfloat max = Var10[0];
	for (GLint i = 1; i < 10; ++i)
	{
		if (max < Var10[i]) max = Var10[i];
	}

	assert(max >= 0.0f);

	m_pVariant[index] = max;

	return max;
}

GLfloat CLod_Quadtree::InitVariant_P2C(GLint level, GLint x, GLint y)
{
	assert(m_data);
	assert(x >= 0 && y >= 0);
	assert(x < (GLint)m_data->GetSize() && y < (GLint)m_data->GetSize());

	GLfloat Var10[10] = { 0 };
	GLint iter = 0;

	GLint index = y * m_data->GetSize() + x;//计算在缓存中的位置

	if (level > 1)
	{
		GLuint Size = 1 << (level - 2);

		Var10[iter++] = InitVariant(level - 1, x + Size, y + Size);
		Var10[iter++] = InitVariant(level - 1, x - Size, y - Size);
		Var10[iter++] = InitVariant(level - 1, x + Size, y - Size);
		Var10[iter++] = InitVariant(level - 1, x - Size, y + Size);
	}

	GLuint Size = 1 << (level - 1);
	GLfloat lu = m_data->GetHeight_P2C(x - Size, y + Size);
	GLfloat ld = m_data->GetHeight_P2C(x - Size, y - Size);
	GLfloat ru = m_data->GetHeight_P2C(x + Size, y + Size);
	GLfloat rd = m_data->GetHeight_P2C(x + Size, y - Size);

	GLfloat left = (lu + ld) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight_P2C(x - Size, y) - left);

	GLfloat right = (ru + rd) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight_P2C(x + Size, y) - right);

	GLfloat up = (ru + lu) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight_P2C(x, y + Size) - up);

	GLfloat down = (rd + ld) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight_P2C(x, y - Size) - down);

	GLfloat center = (lu + rd) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight_P2C(x, y) - center);

	center = (ld + ru) / 2.0f;
	Var10[iter++] = abs(m_data->GetHeight_P2C(x, y) - center);

	if (level > 1)
		assert(10 == iter);
	else
		assert(6 == iter);

	GLfloat max = Var10[0];
	for (GLint i = 1; i < 10; ++i)
	{
		if (max < Var10[i]) max = Var10[i];
	}

	assert(max >= 0.0f);

	m_pVariant[index] = max;

	return max;
}


void CLod_Quadtree::Build()
{
	//m_data->m_D3D->SetIndices(m_pIB);

	//通过2个队列来交替当前级别的节点
	static std::vector<NODE> next_Nodes((1 << (m_nLevel - 1)));
	static std::vector<NODE> cur_Nodes((1 << (m_nLevel - 1)));

	cur_Nodes.clear();
	next_Nodes.clear();

	//根节点
	NODE root(1 << (m_nLevel - 1), 1 << (m_nLevel - 1));
	cur_Nodes.push_back(root);

	static NODE child;
	m_BuildTrianlges = 0;

	//遍历层次级别，从根节点开始
	//并遍历每一个级别的节点
	for (GLuint i = m_nLevel; i > 0; --i)
	{
		m_cur_Level = i;

		for (GLuint j = 0; j < cur_Nodes.size(); ++j)
		{
			if (!NodeIsVisible(cur_Nodes[j]))
			{
				DividNode(cur_Nodes[j]);
				continue;
			}
			if (NodeCanDivid(cur_Nodes[j]))
			{
				DividNode(cur_Nodes[j]);

				int d = 1 << (i - 2);

				child.x = cur_Nodes[j].x - d;
				child.y = cur_Nodes[j].y - d;
				next_Nodes.push_back(child);

				child.x = cur_Nodes[j].x + d;
				child.y = cur_Nodes[j].y - d;
				next_Nodes.push_back(child);

				child.x = cur_Nodes[j].x + d;
				child.y = cur_Nodes[j].y + d;
				next_Nodes.push_back(child);

				child.x = cur_Nodes[j].x - d;
				child.y = cur_Nodes[j].y + d;
				next_Nodes.push_back(child);
			}
			else
			{
				DisableNode(cur_Nodes[j]);
				AttachNode(cur_Nodes[j]);
			}
		}
		cur_Nodes.swap(next_Nodes);
		next_Nodes.clear();
	}
}

void CLod_Quadtree::AttachNode(const NODE & node)
{
	//暂定为6
	if (m_cur_Level > 6) return;

	GLint curLevelCell = 1 << (m_cur_Level - 1);
	assert(m_cur_Level - 1 >= 0);

	//129应改为类属性值或者参数
	GLint fullLevelpoint = 129;//point比cell多1，也就是一排边和点的个数比格子多1

	//逐步插入顶点绘制索引，扇形绘制的数组
	GLuint cellX = node.x;
	GLuint cellY = node.y;

	static GLushort* pos = 0;

	int i = 0;

	//数据坐标为左上角为锚点，X从左到右增加，Y从上到下增加
	pos[i++] = cellY * fullLevelpoint + cellX;//中点
	pos[i++] = (cellY - curLevelCell) * fullLevelpoint + cellX - curLevelCell;//左上
	if (m_AdjctAct[AD_UP])
	{
		pos[i++] = (cellY - curLevelCell) * fullLevelpoint + cellX;//中上
	}
	pos[i++] = (cellY - curLevelCell) * fullLevelpoint + cellX + curLevelCell;//右上
	if (m_AdjctAct[AD_RIGHT])
	{
		pos[i++] = cellY * fullLevelpoint + cellX + curLevelCell;//中右
	}
	pos[i++] = (cellY + curLevelCell) * fullLevelpoint + cellX + curLevelCell;//右下
	if (m_AdjctAct[AD_DOWN])
	{
		pos[i++] = (cellY + curLevelCell) * fullLevelpoint + cellX;//中下
	}
	pos[i++] = (cellY + curLevelCell) * fullLevelpoint + cellX - curLevelCell;//左下
	if (m_AdjctAct[AD_LEFT])
	{
		pos[i++] = cellY * fullLevelpoint + cellX - curLevelCell;//中左
	}
	pos[i++] = (cellY - curLevelCell) * fullLevelpoint + cellX - curLevelCell;//左上

	assert(i <= 10);

	//Draw
}

bool CLod_Quadtree::NodeIsVisible(const NODE & node)
{
	return true;//没有设置平截头体暂时均为可见

	assert(node.x >= 0 && node.x < (int)m_data->GetSize());
	assert(node.y >= 0 && node.y < (int)m_data->GetSize());

	GLuint curSize = (1 << (m_cur_Level - 1));
	GLfloat R = curSize * m_cellSize * 2.6f;
}

bool CLod_Quadtree::NodeCanDivid(const NODE & node)
{

	assert(node.x >= 0 && node.x < (int)m_data->GetSize());
	assert(node.y >= 0 && node.y < (int)m_data->GetSize());

	memset(m_AdjctAct, 1, sizeof(m_AdjctAct));

	D3DXVECTOR3 vecL = m_data->GetPos(node.x, node.y) - m_Cam->GetPos();

	float l = vecL.x*vecL.x + vecL.y*vecL.y + vecL.z*vecL.z;

	unsigned int Size = 1 << m_cur_Level;

	float realSize = Size * m_cellSize;

	float Var = m_pVariant[node.x + m_data->m_Size*node.y];

	bool Divid = m_cur_Level > 1;

	if (m_cur_Level <= CTerrain::e_SectionLevel &&
		realSize*Var / l < m_factor)	 Divid = false;


	int Trans = node.y + Size;
	if (Trans < (int)m_data->m_Size && !m_Bit.IsTrue(node.x, Trans))
	{
		m_AdjctAct[AD_UP] = 0;

		Divid = false;
	}

	Trans = node.y - Size;
	if (Trans >= 0 && !m_Bit.IsTrue(node.x, Trans))
	{
		m_AdjctAct[AD_DOWN] = 0;

		Divid = false;
	}

	Trans = node.x - Size;
	if (Trans >= 0 && !m_Bit.IsTrue(Trans, node.y))
	{
		m_AdjctAct[AD_LEFT] = 0;

		Divid = false;
	}

	Trans = node.x + Size;
	if (Trans < (int)m_data->m_Size && !m_Bit.IsTrue(Trans, node.y))
	{
		m_AdjctAct[AD_RIGHT] = 0;

		Divid = false;
	}

	return Divid;
}

void CLod_Quadtree::DisableNode(const NODE & node)
{
	m_Bit.Set(node.x, node.y, true);
	if (m_cur_Level < 2) return;

	GLuint Size = 1 << (m_cur_Level - 2);
	m_Bit.Set(node.x + Size, node.y + Size, false);
	m_Bit.Set(node.x - Size, node.y - Size, false);
	m_Bit.Set(node.x + Size, node.y - Size, false);
	m_Bit.Set(node.x - Size, node.y + Size, false);
}

void CLod_Quadtree::DividNode(const NODE & node)
{
	m_Bit.Set(node.x, node.y, true);
	if (m_cur_Level < 2) return;

	GLuint Size = 1 << (m_cur_Level - 2);
	m_Bit.Set(node.x + Size, node.y + Size, true);
	m_Bit.Set(node.x - Size, node.y - Size, true);
	m_Bit.Set(node.x + Size, node.y - Size, true);
	m_Bit.Set(node.x - Size, node.y + Size, true);
}

