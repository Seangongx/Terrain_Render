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
//	register GLint All_Level;//ע��˴���û������
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
//	//ע��getpos����������x��Ϊx������,z��Ϊy��������
//	GLfloat cellX = m_data->GetPos(1, 0).x - m_data->GetPos(0, 0).x;
//	GLfloat cellY = m_data->GetPos(0, 1).z - m_data->GetPos(0, 0).z;
//	cellX > cellY ? m_cellSize = cellX : m_cellSize = cellY;
//
//	//TODO:
//	//����m_data����������������
//
//}

void CLod_Quadtree::Init_P2C(CTerrain * T, CCamera* C)
{
	m_data = T;
	m_Cam = C;
	m_Bit.Create(T->GetSize(), T->GetSize());

	safe_delete_array(m_pVariant);

	GLuint input = T->GetSize();
	register GLint All_Level;//ע��˴���û������
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
	//����m_data����������������
	

}

GLfloat CLod_Quadtree::InitVariant(GLint level, GLint x, GLint y)
{
	assert(m_data);
	assert(x >= 0 && y >= 0);
	assert(x < (GLint)m_data->GetSize() && y < (GLint)m_data->GetSize());

	GLfloat Var10[10] = { 0 };
	GLint iter = 0;

	GLint index = x * m_data->GetSize() + y;//�����ڻ����е�λ��

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

	GLint index = y * m_data->GetSize() + x;//�����ڻ����е�λ��

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

	//ͨ��2�����������浱ǰ����Ľڵ�
	static std::vector<NODE> next_Nodes((1 << (m_nLevel - 1)));
	static std::vector<NODE> cur_Nodes((1 << (m_nLevel - 1)));

	cur_Nodes.clear();
	next_Nodes.clear();

	//���ڵ�
	NODE root(1 << (m_nLevel - 1), 1 << (m_nLevel - 1));
	cur_Nodes.push_back(root);

	static NODE child;
	m_BuildTrianlges = 0;

	//������μ��𣬴Ӹ��ڵ㿪ʼ
	//������ÿһ������Ľڵ�
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
	//�ݶ�Ϊ6
	if (m_cur_Level > 6) return;

	GLint curLevelCell = 1 << (m_cur_Level - 1);
	assert(m_cur_Level - 1 >= 0);

	//129Ӧ��Ϊ������ֵ���߲���
	GLint fullLevelpoint = 129;//point��cell��1��Ҳ����һ�űߺ͵�ĸ����ȸ��Ӷ�1

	//�𲽲��붥��������������λ��Ƶ�����
	GLuint cellX = node.x;
	GLuint cellY = node.y;

	static GLushort* pos = 0;

	int i = 0;

	//��������Ϊ���Ͻ�Ϊê�㣬X���������ӣ�Y���ϵ�������
	pos[i++] = cellY * fullLevelpoint + cellX;//�е�
	pos[i++] = (cellY - curLevelCell) * fullLevelpoint + cellX - curLevelCell;//����
	if (m_AdjctAct[AD_UP])
	{
		pos[i++] = (cellY - curLevelCell) * fullLevelpoint + cellX;//����
	}
	pos[i++] = (cellY - curLevelCell) * fullLevelpoint + cellX + curLevelCell;//����
	if (m_AdjctAct[AD_RIGHT])
	{
		pos[i++] = cellY * fullLevelpoint + cellX + curLevelCell;//����
	}
	pos[i++] = (cellY + curLevelCell) * fullLevelpoint + cellX + curLevelCell;//����
	if (m_AdjctAct[AD_DOWN])
	{
		pos[i++] = (cellY + curLevelCell) * fullLevelpoint + cellX;//����
	}
	pos[i++] = (cellY + curLevelCell) * fullLevelpoint + cellX - curLevelCell;//����
	if (m_AdjctAct[AD_LEFT])
	{
		pos[i++] = cellY * fullLevelpoint + cellX - curLevelCell;//����
	}
	pos[i++] = (cellY - curLevelCell) * fullLevelpoint + cellX - curLevelCell;//����

	assert(i <= 10);

	//Draw
}

bool CLod_Quadtree::NodeIsVisible(const NODE & node)
{
	return true;//û������ƽ��ͷ����ʱ��Ϊ�ɼ�

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

