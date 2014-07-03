/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_filterinputstream.cpp
* @version     
* @brief      
* @author   ylh
* @date     2014-07-03
* @note 
*
*  1. 2014-07-03 duye Created this file
* 
*/

#include <g_filterinputstream.h>

using namespace GCommon;



FilterInputStream::FilterInputStream()
{
}

FilterInputStream::~FilterInputStream()
{
}

GInt32 FilterInputStream::available()
{
	return 0;
}

void FilterInputStream::close()
{
}

void FilterInputStream::mark(GInt32 iReadlimit)
{
}

bool FilterInputStream::markSupported()
{
	return false;
}

GInt32 FilterInputStream::read()
{
	return -1;
}

GInt32 FilterInputStream::read(GInt8 * pBuffer,GInt32 iLen)
{
	return -1;
}

GInt32 FilterInputStream::read(GInt8 * pBuffer,GInt32 iOffset,GInt32 iLen)
{
	return -1;
}

void FilterInputStream::reset()
{
}

GInt32 FilterInputStream::skip(GInt32 iNum)
{
	return 0;	
}
