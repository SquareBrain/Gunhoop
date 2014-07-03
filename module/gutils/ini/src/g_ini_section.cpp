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

IniSection::IniSection(const std::string& sectionName)
{
    setSectionName(sectionName);    
}

IniSection::~IniSection()
{
}

void IniSection::setSectionName(const std::string& sectionName)
{
    m_sectionName = sectionName;
}

const std::string& IniSection::getSectionName() const
{
    return m_sectionName;
}

GResult IniSection::addPara(const std::string& para, const std::string& value)
{
    GCommon::AutoLock autoLock(m_mapLock);
    
    KeyValueMap::iterator iter = m_keyValueMap.find(para);
    if (iter != m_keyValueMap.end())
    {
        return G_NO;
    }
    
    m_keyValueMap.insert(std::make_pair(para, value)); 

    return G_YES;
}

GResult IniSection::delPara(const std::string& para)
{
    GCommon::AutoLock autoLock(m_mapLock);
    
    KeyValueMap::iterator iter = m_keyValueMap.find(para);
    if (iter == m_keyValueMap.end())
    {
        return G_NO;
    }
    
    m_keyValueMap.erase(iter);

    return G_YES;
}

GResult IniSection::updatePara(const std::string& para, const std::string& value)
{
    GCommon::AutoLock autoLock(m_mapLock);
    
    KeyValueMap::iterator iter = m_keyValueMap.find(para);
    if (iter == m_keyValueMap.end())
    {
        return G_NO;
    }
    
    iter->second = value;

    return G_YES;   
}

G_NS_END
