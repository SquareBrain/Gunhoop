/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_logger_impl.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-07-01
* @note 
*
*  1. 2014-07-01 duye Created this file
* 
*/
#pragma once

#include <map>
#include <string>
#include <g_system.h>
#include <g_ini.h>

/**
 * system log level
 */	
typedef enum 
{
	LOG_NULL,
	LOG_ERROR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG,
	LOG_TRACE
}GLogLevel;

/**
 * system log print output content
 */	
typedef enum 
{
	PRINT_BASIC,
	PRINT_MORE,
	PRINT_FULL,
}GPrintLevel;

/**
 * where to print
 */
typedef enum
{
	SAVE_STDOUT
	SAVE_STDERR,
	SAVE_FILE
} GSaveWay;

/**
 * system log global configuration rule
 */	
class GlobalRule
{
public:
	GlobalRule();
	~GlobalRule();

	void setTopLogLevel(const GLogLevel& logLevel);
	const GLogLevel& getTopLogLevel() const;

	void setMaxFileNum(const GUint32 maxFileNum);
	GUint32 getMaxFileNum() const;

	void setMaxFileSize(const GUint64 maxFileSize);
	GUint64 getMaxFileSize() const;

	void setAutoWordWrap(const bool isAutoWordWrap);
	bool isAutoWordWrap() const;    

private:
	GLogLevel     	m_topLogLevel;
	GUint32         m_maxFileNum;
	GUint64			m_maxFileSize;		
	bool            m_isAutoWordWrap;
};

/**
 * system log module configuration rule
 */
class ModuleRule
{
public:
	ModuleRule();
	~ModuleRule();

	void setModuleName(const std::string& moduleName);
	const std::string& getModuleName() const;   

	void setLogLevel(const GLogLevel& logLevel);
	const GLogLevel& getLogLevel() const;

	void setPrintLevel(const GPrintLevel& printLevel);
	const GPrintLevel& getPrintLevel() const;   
	
	void setSaveWay(const GSaveWay& saveWay);
	const GSaveWay& getSaveWay() const;
	
	void setFilePrefix(const std::string& filePrefix);
	const std::string& getFilePrefix() const;
	
	void setFilePath(const std::string& filePath);
	const std::string& getFilePath() const;

private:
	std::string     m_moduleName;
	GLogLevel       m_logLevel;
	GPrintLevel     m_printLevel;
	GSaveWay		m_saveWay;
	std::string		m_filePrefix;
	std::string		m_filePath;
};

/**
 * system log configuration
 */	
class GLoggerImpl
{
public:
	// <module_name, module_rule>
	typedef std::map<std::string, ModuleRule*> GModuleRuleMap;
	typedef std::map<GLogLevel, const std::string> GLogLevelMap;
    
public:
	GLoggerImpl();
	~GLoggerImpl();

	static GLoggerImpl* GetInstance();

	GResult init();
	GResult uninit();

	/**
	 * @brief print log
	 * @param [in] logLevel : log level
	 * @param [in] module : module name
	 * @param [in] file : file path
	 * @param [in] line : line number
	 * @param [in] function : function name
	 * @param [in] log : log content
	 */	 	
	void printLog(const GLogLevel logLevel, 
		const GInt8* module, 
		const GInt8* file, 
		const GUint32 line, 
		const GInt8* function,
		const GInt8* args, ...); 
        
	/**
	 * @brief get error string
	 * @return error string
	 */	
	GInt8* getError();    

private:
	GResult findModuleRule(const std::string& moduleName, ModuleRule*& moduleRule);
	void setError(const GInt8* args, ...);

private:
	GLogLevelMap        m_logLevelMap;
	GlobalRule          m_globalRule;
	GModuleRuleMap      m_moduleRuleMap;
	GInt8               m_error[G_ERROR_BUF_SIZE];
};
