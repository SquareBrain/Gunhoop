/***********************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini_section.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-03-15
* @note 
*
*  2. 2014-06-26 duye removal to gohoop project
*
*  1. 2014-03-15 duye Created this file
* 
*/

#include <g_ini_section.h>

G_NS_GUTILS_BEG

IniSection::IniSection()
{
}

IniSection::~IniSection()
{
}

void IniSection::addKeyValue(const std::string& key, const std::string& value)
{
    m_keyValueMap.insert(std::make_pair(key, value)); 
}

const KeyValueMap& IniSection::getKeyValueMap() const
{
    return m_keyValueMap;
}

KeyValueMap& IniSection::getKeyValueMap()
{
    return m_keyValueMap;
}

G_NS_END
