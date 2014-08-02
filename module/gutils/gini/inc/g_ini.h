/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini.h
* @version     
* @brief      
* @author	duye
* @date		2013-11-15
* @note 
*
*  2. 2014-06-24 duye Migration to Gohoop project
*  1. 2013-11-15 duye Created this file
* 
*/
#pragma once

#include <map>
#include <string>
#include <g_system.h>

class GIniSection;

/** 
* @brief ini file section map
* <section_name, ini_section>
*/
typedef std::map<std::string, GIniSection*> GIniSectionMap;

/** 
 * @brief POSIX condition wrapper
 */
class GIniFile
{
public:
	GIniFile();

	/**
	 * @brief constructor
	 * @param [in] filePath : file path
	 * @note 
	 */ 
	GIniFile(const std::string& filePath);

	~GIniFile();

	/**
	 * @brief load file
	 * @param [in] filePath : file path
	 * @return G_YES/G_NO
	 * @note 
	 */
	GResult loadFile(const std::string& filePath);

	/**
	 * @brief import data from buffer
	 * @param [in] data : file data
	 * @return G_YES/G_NO
	 * @note 
	 */     
	GResult importData(const std::string& data);

	/**
	 * @brief import data from buffer
	 * @param [in] data : file data
	 * @param [in] length : data length
	 * @return G_YES/G_NO
	 * @note 
	 */     
	GResult importData(const GInt8* data, const GUint64 length);    

	/**
	 * @brief get value
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
	 * @brief set value
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
	 * @brief del section
	 * @param [in] section : section name
	 * @param [in] paraName : parameter name
	 * @param [in] value : set value
	 * @return G_YES/G_NO
	 * @note 
	 */ 
	GResult delSection(const std::string& section);

	/**
	 * @brief del parameter
	 * @param [in] section : section name
	 * @param [in] paraName : parameter name
	 * @param [in] value : set value
	 * @return G_YES/G_NO
	 * @note 
	 */
	GResult delPara(const std::string& section, const std::string& paraName); 

	/**
	 * @brief save configuration to file
	 * @return G_YES/G_NO
	 * @note 
	 */
	GResult saveFile();  

	/**
	 * @brief save configuration to file
	 * @return G_YES/G_NO
	 * @note 
	 */ 
	GResult saveFile(const std::string& filePath);    

	/**
	 * @brief get error string
	 * @return error string
	 * @note 
	 */
	GInt8* getError();     

private:
	void cleanIniSectionMap();

	GResult parserSection(const GInt8* data, 
		const GUint64 length, 
		GUint64& offset);

	GResult getOneLine(const GInt8* data, 
		const GUint64 length, 
		std::string& lineStr);  

	void setError(const GInt8* args, ...);

private:
	std::string         m_filePath;
	GIniSectionMap      m_iniSectionMap;
	GMutex              m_mapMutex;
	GInt8               m_error[G_ERROR_BUF_SIZE];
};
