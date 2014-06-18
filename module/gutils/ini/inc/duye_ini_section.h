/***********************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		duye_ini_section.h
* @version     
* @brief      
* @author   duye
* @date     2014-03-15
* @note 
*
*  1. 2014-03-15 duye Created this file
* 
*/

#pragma once

#include <map>
#include <string>
#include <duye/util/inc/duye_util_def.h>

DUYE_UTIL_NS_BEG

typedef std::map<std::string, std::string> KeyValueMap;

class Section
{
public:
    Section();
    ~Section();

    void AddKeyValue(const std::string& key, const std::string& value);

    const KeyValueMap& GetKeyValueMap() const;

private:
    KeyValueMap     m_keyValueMap;
};

DUYE_UTIL_NS_END
