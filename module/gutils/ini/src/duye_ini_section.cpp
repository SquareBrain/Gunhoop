/***********************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		duye_ini.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-03-15
* @note 
*
*  1. 2014-03-15 duye Created this file
* 
*/

#include <duye/util/ini/inc/duye_ini_section.h>

DUYE_UTIL_NS_BEG

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

DUYE_UTIL_NS_END
