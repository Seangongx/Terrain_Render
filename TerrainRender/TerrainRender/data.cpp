#include "data.h"
#include <vector>

//标志位二维数组操作
CBit::CBit()
{
	m_pBits = NULL;
	m_Row = m_Col = 0;
}
CBit::~CBit()
{
	if (m_pBits)
	{
		delete[] m_pBits;
		m_pBits = NULL;
	}
}
bool CBit::Create(GLuint R, GLuint C)
{
	m_Row = R, m_Col = C;

	if (m_pBits) delete[] m_pBits;

	GLuint size = R * C / 32;

	if (R*C % 32) size += 1;

	m_pBits = new GLuint[size];

	memset(m_pBits, 0, size * 4);

	return m_pBits != NULL;
}
void CBit::Reset()
{
	GLuint size = m_Row * m_Col / 32;

	if (m_Row*m_Col % 32) size += 1;

	memset(m_pBits, 0, size * 4);
}

//CTerrain类相关操作
CTerrain::CTerrain()
{
	m_pVerticesStream = NULL;
	m_pIndicesStream = NULL;
	m_pVB = NULL;
	//m_pEB = NULL;
	m_size = 0;
}

CTerrain::~CTerrain()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	m_pVerticesStream = NULL;
	m_pIndicesStream = NULL;
	m_pVB = NULL;
}

CTerrain::CTerrain(const std::string verticesfile)
{
	m_vertices.LoadFile(verticesfile, CTerrainFile::VERTICES);
	m_pVerticesStream = (GLdouble*)m_vertices.GetStream();
	m_pIndicesStream = NULL;
	m_pVB = m_pVerticesStream;
	//m_pEB = NULL;
	m_size = sqrt(m_vertices.GetRow());
}

CTerrain::CTerrain(const std::string verticesfile, const std::string indicesfile)
{
	m_vertices.LoadFile(verticesfile, CTerrainFile::VERTICES);
	m_indices.LoadFile(indicesfile, CTerrainFile::INDICES);
	m_pVerticesStream = (GLdouble*)m_vertices.GetStream();
	m_pIndicesStream = (GLuint*)m_indices.GetStream();
	m_pVB = m_pVerticesStream;
	//m_pEB = NULL;
	m_size = sqrt(m_vertices.GetRow());
}

bool CTerrain::Create(GLuint size)
{
	unsigned int input = size;
	register int result;
	_asm bsr eax, input
	_asm mov result, eax
	unsigned int test = ~(1 << result);

	assert(((test & input) == 0 || (test & input) == 1) && "地图大小设置错误，请确保地图的大小必须是2的幂或者+1");
	//assert(size >= (1 << e_SectionLevel) && "地图大小设置错误，请确保地图的大小要大于等于最小单位为地形块的大小");

	m_size = size;//可能地图并没有文件读取的那么大
	if ((test & input) == 0)
	{
		//TODO:地形数据补全成2的幂+1
		++m_size;
	}

	CreateBuffer();

	bool re = true;//其他属性加载失败则返回false

	return true;
}

bool CTerrain::CreateBuffer()
{
	//创建顶点缓存
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.SizeofData(), m_pVerticesStream, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.SizeofData(), m_pIndicesStream, GL_STATIC_DRAW);
	glVertexAttribPointer(0, m_vertices.GetCol(), GL_DOUBLE, GL_TRUE, m_vertices.GetCol() * sizeof(GLdouble), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	return true;
}

bool CTerrain::GenerateObject()
{
	//创建顶点缓存
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.SizeofData(), m_pVerticesStream, GL_STATIC_DRAW);

	glBindVertexArray(0);
	return true;
}


void CTerrain::Render()
{
	glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawElements(GL_TRIANGLES, 3 * m_indices.GetRow(), GL_UNSIGNED_INT, 0);//画出的三角形个数就是索引个数，需要绘制的顶点个数就是三角形个数*3
	//glBindVertexArray(0); // no need to unbind it every time 
}

void CTerrain::SetLodType(CLod* lod)
{
	m_lod = lod;
}

void CTerrain::Delete()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	m_pVerticesStream = NULL;
	m_pIndicesStream = NULL;
	m_pVB = NULL;
}

GLuint CTerrain::GetSize()
{
	return m_size;
}

GLdouble CTerrain::GetHeight(GLint _x, GLint _y)
{
	return m_pVerticesStream[((_y * m_size) + _x) * m_vertices.GetCol() + 2];
}
GLdouble CTerrain::GetHeight_P2C(GLint _x, GLint _y)
{
	return m_pVerticesStream[((_x * m_size) + _y) * m_vertices.GetCol() + m_vertices.GetCol() - 1];
}

glm::vec3 CTerrain::GetPos(GLint _x, GLint _y)
{
	return glm::vec3(_x, _y, GetHeight(_x, _y));
}


#ifdef OLD
#define MAXROWS 32768//Initial value of rows (2pow(7+7+1)) 
#define MAXCOLUMNS 4//Initial value of coloumns
#define READCHAR 30
#define READROW 200

//公用函数
void d_show1vd(double * data, int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		std::cout << i << ":";
		for (j = 0; j < col; j++)
		{
			std::cout << data[i * 3 + j] << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void d_show2vd(double ** data, int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		std::cout << i << ":";
		for (j = 0; j < col; j++)
		{
			std::cout << data[i][j] << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void d_show1vui(unsigned int * data, int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		std::cout << i << ":";
		for (j = 0; j < col; j++)
		{
			std::cout << data[i * 3 + j] << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void d_show2vui(unsigned int ** data, int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		std::cout << i << ":";
		for (j = 0; j < col; j++)
		{
			std::cout << data[i][j] << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//Terrain_Data类函数
Terrain_Data::Terrain_Data(const std::string verticesfile, const std::string indicesfile)
{
	///二维数组形式存储
	verticesMap = NULL;
	indicesMap = NULL;
	///一维数据流存储
	verticesStream = NULL;
	indicesStream = NULL;

	maxRows = MAXROWS;
	maxColumns = MAXCOLUMNS;

	vertices_rows = f_get_rows(verticesfile);
	vertices_columns = f_get_columns(verticesfile) - 1;
	indices_rows = f_get_rows(indicesfile);
	indices_columns = f_get_columns(indicesfile) - 1;

	try {
		verticesStream = new double[vertices_rows * vertices_columns * sizeof(double)]();
	}
	catch (std::bad_alloc) {
		std::cerr << "顶点数据内存分配失败！" << std::endl;
		return;
	}
	try {
		indicesStream = new unsigned int[indices_rows * indices_columns * sizeof(unsigned int)]();
	}
	catch (std::bad_alloc) {
		std::cerr << "索引数据内存分配失败！" << std::endl;
		return;
	}

}
Terrain_Data::Terrain_Data(int vrows, int vcols, int irows, int icols)
{
	///二维数组形式存储
	verticesMap = NULL;
	indicesMap = NULL;
	///一维数据流存储
	verticesStream = NULL;
	indicesStream = NULL;

	maxRows = MAXROWS;
	maxColumns = MAXCOLUMNS;

	vertices_rows = vrows; vertices_columns = vcols;
	indices_rows = irows; indices_columns = icols;
	try {
		verticesStream = new double[vertices_rows * vertices_columns * sizeof(double)]();
	}
	catch (std::bad_alloc) {
		std::cerr << "顶点数据内存分配失败！" << std::endl;
		return;
	}
	try {
		indicesStream = new unsigned int[indices_rows * indices_columns * sizeof(unsigned int)]();
	}
	catch (std::bad_alloc) {
		std::cerr << "索引数据内存分配失败！" << std::endl;
		return;
	}

	/*c动态开辟写法
	verticesMap = (double**)malloc(vertices_rows * sizeof(double*));//注意sizeof的double*
	indicesMap = (unsigned int**)malloc(indices_rows * sizeof(unsigned int*));

	int i = 0, j = 0;
	for (i = 0; i < vertices_rows; i++)
	{
		verticesMap[i] = (double*)malloc(vertices_columns * sizeof(double));
		for (j = 0; j < vertices_columns; j++)
		{
			verticesMap[i][j] = DBL_MIN;
		}
	}
	for (i = 0; i < indices_rows; i++)
	{
		indicesMap[i] = (unsigned int*)malloc(indices_columns * sizeof(unsigned int));
		for (j = 0; j < indices_columns; j++)
		{
			indicesMap[i][j] = INT_MIN;
		}
	}*/

}
Terrain_Data::~Terrain_Data()
{
	if (Data_State() == 1)
	{
		Clear_Data();
	}
}
//Terrain数据装载状态
int Terrain_Data::Data_State()
{
	/*
	if (verticesMap[0][0] == DBL_MIN || indicesMap[0][0] == INT_MIN)
	{
		cout << "data empty!" << endl;
		return;
	}
	*/

	if (verticesMap == NULL || indicesMap == NULL)
	{
		std::cout << "data empty!" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "data loaded!" << std::endl;
		return 1;
	}
}
//Terrain数据清理
void Terrain_Data::Clear_Data()
{
	/*
	for (int i = 0; i < vertices_rows; i++)
	{
		delete[] verticesMap[i];
	}
	delete[] verticesMap;
	verticesMap = NULL;

	for (int i = 0; i < indices_rows; i++)
	{
		delete[] indicesMap[i];
	}
	delete[] indicesMap;
	indicesMap = NULL;
	*/

	delete[] verticesStream; verticesStream = NULL;
	delete[] indicesStream; indicesStream = NULL;
}
//Terrain数据读文件载入
double** Terrain_Data::Load_Vertices(const std::string filename)
{
	char num[READCHAR];//use atol
	//char rowstring[READROW];
	std::string rowstring;
	int rowcount = 0;
	int coloumncount = 0;
	int flag = 0;//whole string count
	int i = 0;//substring count

	std::ifstream fin(filename, std::ios::in);
	if (!fin) {
		std::cout << "The file is not exist!" << std::endl;
		return NULL;
	}
	while (getline(fin, rowstring))
	{
		while (rowstring[flag] != '\0' &&  rowstring[flag] != '\n')
		{
			while (rowstring[flag] != '\0' && rowstring[flag] != ' ' &&  rowstring[flag] != '\n')
			{
				num[i] = rowstring[flag];
				i++;
				flag++;
			}
			num[i] = '\0';
			i = 0;
			verticesMap[rowcount][coloumncount] = atol(num);
			coloumncount++;
			while (rowstring[flag] != '\0' && rowstring[flag] == ' ' && rowstring[flag] != '\n')
			{
				flag++;
			}
		}
		rowcount++;
		coloumncount = 0;
		flag = 0;
	}

	fin.close();
	return verticesMap;
}
double* Terrain_Data::Load_1d_Vertices(const std::string filename)
{
	std::string rowstring;
	std::vector<std::string> temp;
	int count = 0;

	std::ifstream fin(filename, std::ios::in);
	if (!fin) {
		std::cout << "The file is not exist!" << std::endl;
		return NULL;
	}
	while (getline(fin, rowstring))
	{
		if (rowstring.find("\t") && rowstring.find(" ") == -1)//判断不同文件格式，有空格有制表符区分
			temp = str_split(rowstring, "\t");
		else
			temp = str_split(rowstring, " ");
		for (int i = 0; i < temp.size() - 1; i++)
		{
			verticesStream[count++] = stod(temp[i]);
		}
		temp.clear();
	}

	fin.close();
	return verticesStream;

}
unsigned int** Terrain_Data::Load_Indices(const std::string filename)
{
	char num[READCHAR];
	//char rowstring[READROW];
	std::string rowstring;
	int rowcount = 0;
	int coloumncount = 0;
	int flag = 0;//whole string count
	int i = 0;//substring count

	std::ifstream fin(filename, std::ios::in);
	if (!fin) {
		std::cout << "The file is not exist!" << std::endl;
		return NULL;
	}
	while (getline(fin, rowstring))
	{
		while (rowstring[flag] != '\0' &&  rowstring[flag] != '\n')
		{
			while (rowstring[flag] != '\0' && rowstring[flag] != ' ' &&  rowstring[flag] != '\n')
			{
				num[i] = rowstring[flag];
				i++;
				flag++;
			}
			num[i] = '\0';
			i = 0;
			indicesMap[rowcount][coloumncount] = atoi(num);
			coloumncount++;
			while (rowstring[flag] != '\0' && rowstring[flag] == ' ' && rowstring[flag] != '\n')
			{
				flag++;
			}
		}
		rowcount++;
		coloumncount = 0;
		flag = 0;
	}

	fin.close();
	return indicesMap;
}
unsigned int * Terrain_Data::Load_1ui_Indices(const std::string filename)
{
	std::string rowstring;
	std::vector<std::string> temp;
	int count = 0;

	std::ifstream fin(filename, std::ios::in);
	if (!fin) {
		std::cout << "The file is not exist!" << std::endl;
		return NULL;
	}
	while (getline(fin, rowstring))
	{
		if (rowstring.find("\t") && rowstring.find(" ") == -1)//判断不同文件格式，有空格有制表符区分
			temp = str_split(rowstring, "\t");
		else
			temp = str_split(rowstring, " ");
		for (int i = 0; i < temp.size() - 1; i++)
		{
			indicesStream[count++] = stoi(temp[i]);
		}
		temp.clear();
	}

	fin.close();
	return indicesStream;
}
//Terrain属性值获得
int Terrain_Data::Sizeof_Vertices()//TODO:var types default data dimension is 3
{
	//all of pointer space are 4bytes
	return vertices_rows * 3 * sizeof(double);
}
int Terrain_Data::Sizeof_Indices()//TODO:var types default data dimension is 3
{
	//all of pointer space are 4bytes
	return indices_rows * 3 * sizeof(unsigned);
}
int Terrain_Data::Get_VerticesRows()
{
	return this->vertices_rows;
}
int Terrain_Data::Get_IndicesRows()
{
	return this->indices_rows;
}
//Terrain数据控制台显示（一维数据流或二维数组）
void Terrain_Data::Show_Vertices(int dimension)
{
	if (dimension == 1)
	{
		if (verticesStream == NULL)
		{
			std::cout << "data empty!" << std::endl;
			return;
		}
		d_show1vd(verticesStream, vertices_rows, 3);
	}
	else if (dimension == 2)
	{
		if (verticesMap == NULL || verticesMap[0][0] == DBL_MIN)
		{
			std::cout << "data empty!" << std::endl;
			return;
		}
		d_show2vd(verticesMap, vertices_rows, 3);
	}
	return;
}
void Terrain_Data::Show_Indices(int dimension)
{
	if (dimension == 1)
	{
		if (indicesStream == NULL)
		{
			std::cout << "data empty!" << std::endl;
			return;
		}
		d_show1vui(indicesStream, indices_rows, 3);
	}
	else if (dimension == 2)
	{
		if (indicesMap == NULL || indicesMap[0][0] == INT_MIN)
		{
			std::cout << "data empty!" << std::endl;
			return;
		}
		d_show2vui(indicesMap, indices_rows, 3);
	}
	return;
}
#endif

/*暂时用不到的一些函数
bool d_pick_vertices(double** vertices, double * pick, int rows, int cols)
{
	int i = 0, j = 0, count = 0;
	//int rows = MAXROWS / 2;
	//int cols = MAXCOLUMNS - 1;//一般为3
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
				pick[count++] = vertices[i][j];
		}
	}
	if (count == rows*cols)
		return true;
	else return false;
}

bool d_pick_indices(unsigned int** indices, unsigned * pick, int rows, int cols)
{
	int i = 0, j = 0, count = 0;
	//int rows = pow((sqrt(MAXROWS / 2) - 1), 2) * 2;
	//int cols = MAXCOLUMNS - 1;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			pick[count++] = indices[i][j];
		}
	}
	if (count == rows*cols)
		return true;
	else return false;
}

bool d_copy_vertices(double* vertices, double* copy, int count)
{
	int i = 0;
	for (; i < count; i++)
		copy[i] = vertices[i];
	if (i == count)
		return true;
	else return false;
}

bool d_copy_indices(unsigned int* indices, unsigned int* copy, int count)
{
	int i = 0;
	for (; i < count; i++)
		copy[i] = indices[i];
	if (i == count)
		return true;
	else return false;
}
*/
