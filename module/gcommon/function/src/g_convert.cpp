/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_convert.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-03-09
* @note 
*
*  1. 2014-03-09 duye Created this file
* 
*/
#include <g_convert.h>

using namespace gsys;

namespace gcom {

GResult Convert::convTime(const GInt8* format, 
    GInt8* buffer, 
    const GUint32 size)
{
	return convTime(System::getSysTime(), format, buffer, size);
}

GResult Convert::convTime(const std::string& format, 
    std::string& buffer)
{
	GInt8 timeBuffer[128] = {0};
	if (convTime(format.c_str(), timeBuffer, sizeof(timeBuffer)) != G_YES)
	{
		return G_NO;
	}
	
	buffer.assign(timeBuffer);
	
	return G_YES;
}

GResult Convert::convTime(const GUint64 millisecond, 
    const GInt8* format, 
    GInt8* buffer, 
    const GUint32 size)
{
	if (format == nullptr || buffer == nullptr || size == 0)
	{
		return G_NO;	
	}

	return G_YES;
}

GResult Convert::convTime(const GUint64 millisecond, 
    const std::string format, 
    std::string& buffer)
{
	GInt8 timeBuffer[128] = {0};
	if (convTime(millisecond, format.c_str(), timeBuffer, sizeof(timeBuffer)) != G_YES)
	{
		return G_NO;	
	}
	
	buffer.assign(timeBuffer);
	
	return G_YES;
}

GResult Convert::splitString(const std::string& src_str, const GInt8 split, std::list<std::string> split_list)
{
	GUint64 pre_pos = 0;
	GUint64 pos = 0;
	while ((pos = src_str.find(split, pre_pos)) != std::string::npos)
	{
		split_list.push_back(src_str.substr(pre_pos, pos - pre_pos));
		pre_pos = pos + 1;
	}
	
	if (pre_pos < src_str.size())
	{
		split_list.push_back(src_str.substr(pre_pos, src_str.size - pre_pos));
	}
	
	if (splitList.empty())
	{
		return G_NO;
	}
	
	return G_YES;
}

static GResult Convert::splitString(const std::string& src_str, const std::string split, std::list<std::string> splitList)
{
	GUint64 pre_pos = 0;
	GUint64 pos = 0;
	while ((pos = src_str.find(split, pre_pos)) != std::string::npos)
	{
		split_list.push_back(src_str.substr(pre_pos, pos - pre_pos));
		pre_pos = pos + split.size();
	}
	
	if (pre_pos < src_str.size())
	{
		split_list.push_back(src_str.substr(pre_pos, src_str.size - pre_pos));
	}
	
	if (splitList.empty())
	{
		return G_NO;
	}
	
	return G_YES;	
}
	
}
