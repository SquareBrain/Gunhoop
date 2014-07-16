/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_fileinputstream.cpp
* @version     
* @brief      
* @author   ylh
* @date     2014-07-11
* @note 
*
*  1. 2014-07-11 ylh Created this file
* 
*/

#include <g_fileinputstream.h>

using namespace std;
using namespace GCommon;



FileInputStream::FileInputStream(shared_ptr<GFile> file)
	: m_file(file)
{
}

FileInputStream::FileInputStream(const string filepath)
	: m_file((shared_ptr<GFile>(new GFile(filepath.c_str()))))
{
	m_file->open(G_OPEN_READ);
}

FileInputStream::~FileInputStream()
{
}

GInt32 FileInputStream::available()
{
	// TODO: GFile
	return 0;
}

void FileInputStream::close()
{
	// m_file->close();
	// do nothing
}

GInt32 FileInputStream::read()
{
	GInt8 pBuffer[1] = {0};
	return  this->read(pBuffer, 1);
}

GInt32 FileInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen)
{
	return this->read(pBuffer, iBufferLen, 0, iBufferLen);
}

GInt32 FileInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen, GInt32 iOffset, GInt32 iLen)
{
	if (openCheck() == G_NO)
	{
		return -4;
	}

	if (!pBuffer)
	{
		return -2;
	}
	else if (iOffset < 0 || iLen < 0 ||iBufferLen < iOffset + iLen)
	{
		return -3;
	}
	else if (iLen == 0)
	{
		return 0;
	}

	return m_file->read(pBuffer + iOffset, iLen);
}

GInt64 FileInputStream::skip(GInt64 lNum)
{
	if (openCheck() == G_NO)
	{
		return -4;
	}

	if (lNum == 0)
	{
		return 0;
	}
	if (lNum  < 0)
	{
		return -1;
	}

	// TODO: GFile
	return 0;
}

shared_ptr<GFile> FileInputStream::GetFile()
{
	return m_file;
}

GResult FileInputStream::openCheck()
{
	// TODO: GFile
	return G_NO;
}

