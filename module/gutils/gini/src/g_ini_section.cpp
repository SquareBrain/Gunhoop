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

GIniSection::GIniSection()
{
}

GIniSection::GIniSection(const std::string& sectionName)
{
    m_sectionName = sectionName;
}

GIniSection::~GIniSection()
{
}

GResult GIniSection::getPara(const std::string& para, std::string& value)
{
    GAutoLock autoLock(m_mapMutex);
    
    KeyValueMap::iterator iter = m_keyValueMap.find(para);
    if (iter == m_keyValueMap.end())
    {
        return G_NO;
    }
    
    value.assign(iter->second);

    return G_YES;    
}

GResult GIniSection::setPara(const std::string& para, const std::string& value)
{
    GAutoLock autoLock(m_mapMutex);
    
    KeyValueMap::iterator iter = m_keyValueMap.find(para);
    if (iter == m_keyValueMap.end())
    {
        return G_NO;
    }
    
    iter->second = value;

    return G_YES;   
}

GResult GIniSection::addPara(const std::string& para, const std::string& value)
{
    GAutoLock autoLock(m_mapMutex);
    
    KeyValueMap::iterator iter = m_keyValueMap.find(para);
    if (iter != m_keyValueMap.end())
    {
        return G_NO;
    }
    
    m_keyValueMap.insert(std::make_pair(para, value)); 

    return G_YES;
}

GResult GIniSection::delPara(const std::string& para)
{
    GAutoLock autoLock(m_mapMutex);
    
    KeyValueMap::iterator iter = m_keyValueMap.find(para);
    if (iter == m_keyValueMap.end())
    {
        return G_NO;
    }
    
    m_keyValueMap.erase(iter);

    return G_YES;
}

const GIniSection::KeyValueMap& GIniSection::getkeyValueMap() const
{
    return m_keyValueMap; 
}
