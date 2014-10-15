/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_convert.cpp
* @version     
* @brief      
* @author   duye
* @date	    2014-03-09
* @note 
*
*  1. 2014-03-09 duye Created this file
* 
*/
#include <string.h>
#include <g_convert.h>

namespace gcom {

GResult Convert::splitString(const std::string& src_str, const GInt8 token, std::list<std::string>& split_list)
{
    GUint64 pre_pos = 0;
    GUint64 pos = 0;
    while ((pos = src_str.find(token, pre_pos)) != std::string::npos)
    {
    	split_list.push_back(src_str.substr(pre_pos, pos - pre_pos));
    	pre_pos = pos + 1;
    }
	
    if (pre_pos < src_str.size())
    {
    	split_list.push_back(src_str.substr(pre_pos, src_str.size() - pre_pos));
    }
	
    return split_list.empty() ? G_NO : G_YES;
}

GResult Convert::splitString(const std::string& src_str, const std::string& token, std::list<std::string>& split_list)
{
    GUint64 pre_pos = 0;
    GUint64 pos = 0;
    while ((pos = src_str.find(token, pre_pos)) != std::string::npos)
    {
    	split_list.push_back(src_str.substr(pre_pos, pos - pre_pos));
    	pre_pos = pos + token.size();
    }
	
    if (pre_pos < src_str.size())
    {
    	split_list.push_back(src_str.substr(pre_pos, src_str.size() - pre_pos));
    }
	
    return split_list.empty() ? G_NO : G_YES;	
}

void Convert::toupper(GInt8& ch)
{
    ch = ::toupper(ch);    	
}

void Convert::toupper(GInt8* str)
{
    IS_YES_R(str == nullptr);
    
    for (GUint32 i = 0; i < str.length(); ++i)
    {
    	*(str + i) = ::toupper(*(str + i));
    }
}

void Convert::toupper(std::string& str)
{
    for (GUint32 i = 0; i < str.length(); ++i)
    {
    	str[i] = ::toupper(str[i]);
    }
}

void Convert::tolower(GInt8& ch)
{
    ch = ::tolower(ch);
}

void Convert::tolower(GInt8* str)
{
    IS_YES_R(str == nullptr);
    
    for (GUint32 i = 0; i < str.length(); ++i)
    {
        *(str + i) = ::tolower(*(str + i));	
    }
}

void Convert::tolower(std::string& str)
{
    for (GUint32 i = 0; i < str.length(); ++i)
    {
        str[i] = ::tolower(str[i]);	
    }
}

}
