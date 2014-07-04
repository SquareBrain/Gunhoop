/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini_parser.h
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

#include <map>
#include <string>
#include <g_type.h>
#include <g_lock.h>

G_NS_GUTILS_BEG

class IniSection;

/** 
 * ini file section map
 * <section_name, ini_section>
 */
typedef std::map<std::string, IniSection*> IniSectionMap;

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
     * import data from buffer
     * @param [in] data : file data
     * @return G_YES/G_NO
     * @note 
     */	     
    GResult importData(const std::string& data);

    /**
     * import data from buffer
     * @param [in] data : file data
     * @param [in] length : data length
     * @return G_YES/G_NO
     * @note 
     */	     
    GResult importData(const GInt8* data, const GUint64 length);    

    /**
     * get value
     * @param [in] section : section name
     * @param [in] paraName : parameter name
     * @param [out] value : return value
     * @return G_YES/G_NO
     * @note 
     */	 
    GResult getParaVal(const std::string& section, 
        const std::string& paraName, 
        std::string& value);

    /**
     * set value
     * @param [in] section : section name
     * @param [in] paraName : parameter name
     * @param [in] value : set value
     * @return G_YES/G_NO
     * @note 
     */	 
    GResult setParaVal(const std::string& section, 
        const std::string& paraName, 
        const std::string& value);    
    /**
     * del section
     * @param [in] section : section name
     * @param [in] paraName : parameter name
     * @param [in] value : set value
     * @return G_YES/G_NO
     * @note 
     */	 
    GResult delSection(const std::string& section);
    
    /**
     * del parameter
     * @param [in] section : section name
     * @param [in] paraName : parameter name
     * @param [in] value : set value
     * @return G_YES/G_NO
     * @note 
     */	 
    GResult delPara(const std::string& section, const std::string& paraName); 
    
    /**
     * save configuration to file
     * @return G_YES/G_NO
     * @note 
     */	 
    GResult saveFile();  

    /**
     * save configuration to file
     * @return G_YES/G_NO
     * @note 
     */	 
    GResult saveFile(const std::string& filePath);    

private:
    void cleanIniSectionMap();
    
    GResult parserSection(const GInt8* data, 
        const GUint64 length, 
        GUint64& offset);

    GResult getOneLine(const GInt8* data, 
        const GUint64 length, 
        std::string& lineStr);        

private:
    std::string         m_filePath;
    IniSectionMap       m_iniSectionMap;
    GCommon::Mutex      m_mapLock;
};

G_NS_END
