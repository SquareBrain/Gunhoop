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
*  1. 2014-03-15 duye Created this file
* 
*/

#pragma once

#include <map>
#include <string>
#include <g_type.h>
#include <g_lock.h>

G_NS_GUTILS_BEG

/** 
 * ini file section
 */
class IniSection
{
public:
    /** 
     * key-value map
     */
    typedef std::map<std::string, std::string> KeyValueMap;

public:
    IniSection();

    /**
     * constructor
     * @param [in] sectionName : section name
     * @return KeyValueMap const reference
     * @note 
     */	    
    explicit IniSection(const std::string& sectionName);
    
    ~IniSection();

    /**
     * get value
     * @param [in] para : parameter
     * @param [in] value : value
     * @return G_YES/G_NO
     * @note 
     */	
    GResult getPara(const std::string& para, std::string& value); 

    /**
     * set value
     * @param [in] para : parameter
     * @param [in] value : value
     * @return G_YES/G_NO
     * @note 
     */	
    GResult setPara(const std::string& para, const std::string& value);    

    /**
     * add parameter ane value
     * @param [in] para : parameter
     * @param [in] value : value
     * @return G_YES/G_NO
     * @note 
     */	
    GResult addPara(const std::string& para, const std::string& value);

    /**
     * delete parameter
     * @param [in] para : parameter
     * @return G_YES/G_NO
     * @note 
     */	
    GResult delPara(const std::string& para);

    /**
     * get keyValueMap
     * @return m_keyValueMap 
     * @note 
     */	
    const KeyValueMap& getkeyValueMap() const;   
    
private:
    std::string     m_sectionName;
    KeyValueMap     m_keyValueMap;
    GCommon::Mutex  m_mapLock;
};

G_NS_END
