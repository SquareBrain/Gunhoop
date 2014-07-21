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



FileInputStream::FileInputStream(shared_ptr<GFile> file) throw(std::logic_error)
	: m_file(file)
{
	if (m_file == NULL)
	{
		throw invalid_argument("");
	}
}

FileInputStream::FileInputStream(const string filepath) throw(std::bad_alloc, std::ios_base::failure)
	: m_file((shared_ptr<GFile>(new GFile(filepath.c_str()))))
{
	if (m_file->open(G_OPEN_READ) == G_NO)
	{
		throw ios_base::failure("");
	}
}

FileInputStream::~FileInputStream()
{
}

GInt32 FileInputStream::available() throw(std::ios_base::failure)
{
	// TODO: GFile
	return 0;
}

void FileInputStream::close() throw(std::ios_base::failure)
{
	// m_file->close();
	// do nothing
}

GInt32 FileInputStream::read() throw(std::ios_base::failure, std::logic_error)
{
	GInt8 pBuffer[1] = {0};
	return  this->read(pBuffer, 1);
}

GInt32 FileInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen, GInt32 iOffset, GInt32 iLen) throw(std::ios_base::failure, std::logic_error)
{
	if (openCheck() == G_NO)
	{
		return -4;
	}

	if (!pBuffer)
	{
		throw invalid_argument("");
	}
	else if (iOffset < 0 || iLen < 0 ||iBufferLen < iOffset + iLen)
	{
		throw out_of_range("");
	}
	else if (iLen == 0)
	{
		return 0;
	}

	return m_file->read(pBuffer + iOffset, iLen);
}

GInt64 FileInputStream::skip(GInt64 lNum) throw(std::ios_base::failure)
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
		throw out_of_range("");
	}

	// TODO: GFile
	if (m_file->seek(lNum, G_SEEK_CUR))
	{
		throw ios_base::failure("");
	}
	return 0;
}

shared_ptr<GFile> FileInputStream::GetFile()
{
	return m_file;
}

GResult FileInputStream::openCheck()
{
	// TODO: GFile
	return G_YES;
}
