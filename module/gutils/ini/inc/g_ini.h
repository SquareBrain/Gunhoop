/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
* @note 
*
*  2. 2014-06-24 duye Migration to Gohoop project
*  1. 2013-11-15 duye Created this file
* 
*/

#pragma once

#include <list>
#include <string>
#include <g_type.h>

G_NS_GUTILS_BEG

typedef std::list<Section*> SectionList;

class IniParser
{
public:
    IniParser();
    IniParser(const std::string& filePath);
    ~IniParser();

    D_Result LoadFile(const std::string& filePath);
    D_Result ImportContent(const std::string& content);

    const SectionList& GetSectionList() const;

private:
    SectionList     m_sectionList;
};

G_NS_GUTILS_END