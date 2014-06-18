/***********************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		duye_ini.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
* @note 
*
*  1. 2013-11-15 duye Created this file
* 
*/

#pragma once

#include <list>
#include <string>

#include <duye/util/inc/duye_util_def.h>
#include <duye/util/ini/inc/duye_ini_section.h>

DUYE_UTIL_NS_BEG

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

DUYE_UTIL_NS_END
