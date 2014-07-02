/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
* @note 
*
*  2. 2014-06-24 duye Migration to Gohoop project
*  1. 2013-11-15 duye Created this file
* 
*/
#pragma once

#include <list>
#include <string>
#include <g_type.h>

G_NS_GUTILS_BEG

class IniSection;

/** 
 * ini file section list
 */
typedef std::list<IniSection*> SectionList;

/** 
 * POSIX condition wrapper
 */
class IniParser
{
public:
    IniParser();
    /**
     * constructor
     * @param [in] filePath : file path
     * @note 
     */	    
    IniParser(const std::string& filePath);
    ~IniParser();

    /**
     * load file
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */	 
    GResult loadFile(const std::string& filePath);

    /**
     * load data buffer
     * @param [in] data : file data
     * @return G_YES/G_NO
     * @note 
     */	     
    GResult loadData(const std::string& data);

    /**
     * get ini configuration section with const reference
     * @return const SectionList reference
     * @note 
     */	 
    const SectionList& getSectionList() const;

    /**
     * get ini configuration section with no const
     * @return SectionList reference
     * @note 
     */	 
    SectionList& getSectionList();
    
    /**
     * save file update
     * @return G_YES/G_NO
     * @note 
     */	 
    GResult saveFile() const;    

private:
    std::string     m_filePath;
    SectionList     m_sectionList;
};

G_NS_END
