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

G_NS_GUTILS_BEG

typedef std::map<std::string, std::string> KeyValueMap;

class IniSection
{
public:
    IniSection();
    ~IniSection();

    void addKeyValue(const std::string& key, const std::string& value);

    const KeyValueMap& getKeyValueMap() const;

private:
    KeyValueMap     m_keyValueMap;
};

G_NS_END
