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
 * system log print format
 */	
typedef enum 
{
	PRINT_BASIC,
	PRINT_MORE,
	PRINT_FULL,
}GPrintFormat;

/**
 * where to print
 */
typedef enum
{
	SAVE_STDOUT,
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

	void setAutoWordwrap(const bool isAutoWordwrap);
	bool isAutoWordwrap() const;    

private:
	GLogLevel		m_topLogLevel;
	GUint32			m_maxFileNum;
	GUint64			m_maxFileSize;		
	bool			m_isAutoWordwrap;
};

/**
 * system log module configuration rule
 */
class GModuleRule
{
public:
	GModuleRule();
	~GModuleRule();

	void setModuleName(const std::string& moduleName);
	const std::string& getModuleName() const;   

	void setLogLevel(const GLogLevel& logLevel);
	const GLogLevel& getLogLevel() const;

	void setPrintFormat(const GPrintFormat& printFormat);
	const GPrintFormat& getPrintFormat() const;   
	
	void setSaveWay(const GSaveWay& saveWay);
	const GSaveWay& getSaveWay() const;
	
	void setFilePrefix(const std::string& filePrefix);
	const std::string& getFilePrefix() const;
	
	void setFilePath(const std::string& filePath);
	const std::string& getFilePath() const;

    void setFileName(const std::string& fileName);
    const std::string& getFileName() const;

private:
	std::string		m_moduleName;
	GLogLevel		m_logLevel;
	GPrintFormat	m_printFormat;
	GSaveWay		m_saveWay;
	std::string		m_filePrefix;
	std::string		m_filePath;
    std::string		m_fileName;
};

/**
 * system log file
 */
class GLogFile
{
public:
	explicit GLogFile(const std::string& fileName, 
        const GUint32 maxFileNum,
        const GUint64 maxFileSize);
    
	~GLogFile();
	
	GResult write(const GInt8* data, const GUint64 len);

private:
	GResult open();
	
private:
	gsys::File* 	m_file;
	std::string		m_fileName;
    GUint32			m_maxFileNum;
    GUint32			m_genFileCount;
    GUint64			m_maxFileSize;
    GUint64			m_currFileSize;
};
 
/**
 * system log configuration
 */	
class GLoggerImpl
{
public:
	// <module_name, module_rule>
	typedef std::map<const std::string, GModuleRule*> GModuleRuleMap;
	typedef std::map<const GLogLevel, const std::string> GLogLevelMap;
	typedef std::map<const GPrintFormat, const std::string>	GPrintFormatMap;
	typedef std::map<const GSaveWay, const std::string> GSaveWayMap;
	typedef std::map<const std::string, GLogFile*>	GLogFileMap;
    
public:
	GLoggerImpl();
	~GLoggerImpl();

	static GLoggerImpl* GetInstance();

	GResult init();

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
	void uninit();
	void saveToFile(const GModuleRule* moduleRule, const GInt8* log, const GUint64 len);
	GResult parserLogConf();
	GResult initFile();
	GResult findLogLevel(const GInt8* logLevelStr, GLogLevel& logLevel) const;
	GResult findPrintFormat(const GInt8* printFormatStr, GPrintFormat& printFormat) const;
	GResult findSaveWay(const GInt8* saveWayStr, GSaveWay& saveWay) const;
	const GModuleRule* findModuleRule(const std::string& moduleName) const;
    void getSysTime(GInt8* buffer, const GUint32 size);
	void setError(const GInt8* args, ...);

private:
	GLogLevelMap		m_logLevelMap;
	GPrintFormatMap		m_printFormatMap;
	GSaveWayMap			m_saveWayMap;
	GlobalRule			m_globalRule;
	GModuleRuleMap		m_moduleRuleMap;
	GLogFileMap			m_logFileMap;
	GInt8				m_error[G_ERROR_BUF_SIZE];
};
