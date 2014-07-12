/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_logger_impl.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-07-01
* @note 
*
*  1. 2014-07-01 duye Created this file
* 
*/
#pragma once

#include <map>
#include <string>
#include <g_type.h>
#include <g_ini_file.h>

/**
 * system log level
 */	
typedef enum 
{
    LOG_ERROR = 0,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_TRACE
}LogLevel;

/**
 * system log print output content
 */	
typedef enum 
{
    PRINT_BASIC = 0,
    PRINT_MORE,
    PRINT_FULL,
}PrintLevel;

/**
 * system log global configuration rule
 */	
class GlobalRule
{
public:
    GlobalRule();
    ~GlobalRule();

    void setFileName(const std::string& fileName);
    const std::string getFileName() const;
    
    void setFileSize(const GUint64 fileSize);
    GUint64 getFileSize() const;

    void setFileCount(const GUint32 fileCount);
    GUint32 getFileCount() const;

    void setWordWrap(const bool isWordWrap);
    bool isWordWrap() const;    

private:
    std::string     m_fileName;
    GUint64         m_fileSize;
    GUint32         m_fileCount;
    bool            m_isWordWrap;
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

    void setLogLevel(const LogLevel logLevel);
    const LogLevel& getLogLevel() const;

    void setPrintLevel(const PrintLevel printLevel);
    const PrintLevel& getPrintLevel() const;   

private:
    std::string     m_moduleName;
    LogLevel        m_logLevel;
    PrintLevel      m_printLevel;
};

/**
 * system log configuration
 */	
class Logger
{
public:
    // <module_name, module_rule>
    typedef std::map<std::string, ModuleRule*> ModuleRuleMap;
    typedef std::map<LogLevel, const GInt8*> LogLevelMap;
    
public:
    Logger();
    ~Logger();

    static Logger* GetInstance();

    GResult init();
    GResult uninit();

    /**
     * print log
     * @param [in] logLevel : log level
     * @param [in] module : module name
     * @param [in] file : file path
     * @param [in] line : line number
     * @param [in] function : function name
     * @param [in] log : log content
     */	 	
    void printLog(const LogLevel logLevel, 
        const GInt8* module, 
        const GInt8* file, 
        const GUint32 line, 
        const GInt8* function,
        const GInt8* format,
        va_list vaList); 
        
    /**
     * get error string
     * @return error string
     */	
    GInt8* getError();    

private:
    GResult findModuleRule(const std::string& moduleName, ModuleRule*& moduleRule);
    void setError(const GInt8* args, ...);

private:
    LogLevelMap         m_logLevelMap;
    GlobalRule          m_globalRule;
    ModuleRuleMap       m_moduleRuleMap;
    IniFile             m_iniFile;
    GInt8               m_error[G_ERROR_BUF_SIZE];
};
