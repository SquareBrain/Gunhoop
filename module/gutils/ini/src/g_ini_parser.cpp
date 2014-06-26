/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini_parser.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
* @note 
*
*  2. 2014-06-26 duye removal to gohoop project
*
*  1. 2013-11-15 duye Created this file
* 
*/

#include <g_ini.h>

G_NS_GUTILS_BEG

IniParser::IniParser()
{
}

IniParser::IniParser(const std::string& filePath)
{
    loadFile(m_filePath);
}

IniParser::~IniParser()
{
}

GResult IniParser::loadFile(const std::string& filePath)
{
    m_inifile.
    
    return G_YES;
}

GResult IniParser::loadData(const std::string& fileData)
{
    return G_YES;
}

const SectionList& IniParser::getSectionList() const
{
    return m_sectionList;
}

SectionList& IniParser::getSectionList()
{
    return m_sectionList;
}

GResult IniParser::saveFile() const
{
    return G_YES;
}

G_NS_END
