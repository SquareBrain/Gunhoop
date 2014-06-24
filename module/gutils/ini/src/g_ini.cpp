/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		duye_ini.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
* @note 
*
*  1. 2013-11-15 duye Created this file
* 
*/

#include <duye/util/ini/inc/duye_ini.h>

DUYE_UTIL_NS_BEG

IniParser::IniParser()
{
}

IniParser::IniParser(const std::string& filePath)
{
    LoadFile(filePath);
}

IniParser::~IniParser()
{
}

D_Result IniParser::LoadFile(const std::string& filePath)
{
    return D_SUCCESS;
}

D_Result IniParser::ImportContent(const std::string& content)
{
    return D_SUCCESS;
}

const SectionList& IniParser::GetSectionList() const
{
    return m_sectionList;
}

DUYE_UTIL_NS_END
