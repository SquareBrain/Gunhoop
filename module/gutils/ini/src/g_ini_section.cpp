/***********************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-03-15
* @note 
*
*  2. 2014-06-26 duye Created this file
*
*  1. 2014-03-15 duye Created this file
* 
*/

#include <g_ini_section.h>

G_NS_GUTILS_BEG

Section::Section()
{
}

Section::~Section()
{
}

void Section::AddKeyValue(const std::string& key, const std::string& value)
{
    m_keyValueMap.insert(std::make_pair(key, value)); 
}

const KeyValueMap& Section::GetKeyValueMap() const
{
    return m_keyValueMap;
}

G_NS_END
