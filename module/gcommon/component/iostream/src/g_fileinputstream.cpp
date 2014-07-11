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



FileInputStream::FileInputStream(File file)
{
}

FileInputStream::FileInputStream(string filepath)
{
}

FileInputStream::~FileInputStream()
{
}

GInt32 FileInputStream::available()
{
	return 0;
}

void FileInputStream::close()
{
}

GInt32 FileInputStream::read()
{
	return  -1;
}

GInt32 FileInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen)
{
	return read(pBuffer, iBufferLen, 0, iBufferLen);
}

GInt32 FileInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen, GInt32 iOffset, GInt32 iLen)
{
	return  -1;
}

GInt64 FileInputStream::skip(GInt64 lNum)
{
	return -1;
}

