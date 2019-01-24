#include "file.h"

CTerrainFile::CTerrainFile()
{
	m_filerow = 0;
	m_filecol = 0;
	m_pFileStream = NULL;
	m_eFileType = EMPTY;

}

CTerrainFile::CTerrainFile(const std::string filename, EType filetype)
{
	m_pFileStream = NULL;
	m_eFileType = filetype;

	LoadFile(filename, filetype);
}

CTerrainFile::~CTerrainFile()
{
	if (m_pFileStream != NULL)
	{
		delete[] m_pFileStream; m_pFileStream = NULL;
	}
}

void CTerrainFile::LoadFile(const std::string filename, EType filetype)
{
	m_filerow = f_get_rows(filename);
	m_filecol = f_get_columns(filename) - 1;
	m_eFileType = filetype;

	if (filetype == VERTICES)
	{
		try {
			m_pFileStream = new double[m_filerow * m_filecol * sizeof(double)]();
			Load_1d_Vertices(filename);
		}
		catch (std::bad_alloc) {
			std::cerr << "顶点数据内存分配失败！" << std::endl;
			return;
		}
	}
	else if (filetype == INDICES)
	{
		try {
			m_pFileStream = new unsigned int[m_filerow * m_filecol * sizeof(unsigned int)]();
			Load_1ui_Indices(filename);
		}
		catch (std::bad_alloc) {
			std::cerr << "索引数据内存分配失败！" << std::endl;
			return;
		}
	}

	//TODO:Texture and Light

}


int CTerrainFile::SizeofData()
{
	if (m_eFileType == VERTICES)
		return m_filerow * m_filecol * sizeof(double);
	else if (m_eFileType == INDICES)
		return m_filerow * m_filecol * sizeof(unsigned int);
	//TODO:
}

void CTerrainFile::Load_1d_Vertices(const std::string filename)
{
	std::string rowstring;
	std::vector<std::string> temp;
	int count = 0;
	double *verticesStream = (double*)m_pFileStream;

	std::ifstream fin(filename, std::ios::in);
	if (!fin) {
		std::cerr << "The file is not exist!" << std::endl;
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
	verticesStream == NULL;
}

void CTerrainFile::Load_1ui_Indices(const std::string filename)
{
	std::string rowstring;
	std::vector<std::string> temp;
	int count = 0;
	unsigned int* indicesStream = (unsigned int*)m_pFileStream;

	std::ifstream fin(filename, std::ios::in);
	if (!fin) {
		std::cerr << "The file is not exist!" << std::endl;
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
	indicesStream = NULL;
}