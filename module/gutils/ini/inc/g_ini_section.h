/***********************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini_section.h
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

#pragma once

#include <map>
#include <string>
#include <g_type.h>

G_NS_GUTILS_BEG

/** 
 * key-value map
 */
typedef std::map<std::string, std::string> KeyValueMap;

/** 
 * ini file section
 */
class IniSection
{
public:
    IniSection();
    ~IniSection();

    /**
     * add key-value
     * @param [in] key : attribute name
     * @param [in] value : attribute value
     * @note 
     */	
    void addKeyValue(const std::string& key, const std::string& value);
    
    /**
     * get key-value map const reference
     * @param [in] filePath : file path
     * @return KeyValueMap const reference
     * @note 
     */	
    const KeyValueMap& getKeyValueMap() const;

    /**
     * get key-value map reference
     * @param [in] filePath : file path
     * @return KeyValueMap reference
     * @note 
     */	
    KeyValueMap& getKeyValueMap(); 

private:
    KeyValueMap     m_keyValueMap;
};

G_NS_END
