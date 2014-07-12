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
#include <stdarg.h>
#include <stdio.h>
#include <g_file.h>
#include <g_logger_impl.h>

// the max buffer size of in one line
static const GUint64 DEF_ONE_LINE_BUF_SIZE = 1024;

// the configuration file name
static const GInt8* DEF_CONF_FILE_NAME = "glog.conf";

Logger::Logger()
{
    m_error[0] = 0;
}

Logger::~Logger()
{
}

Logger* Logger::GetInstance()
{
    static Logger logger;
    return &logger;
}

GResult Logger::init()
{    
    m_logLevelMap.insert(std::make_pair(LOG_ERROR, "ERROR"));
    m_logLevelMap.insert(std::make_pair(LOG_WARN, "WARN"));
    m_logLevelMap.insert(std::make_pair(LOG_INFO, "INFO"));
    m_logLevelMap.insert(std::make_pair(LOG_DEBUG, "DEBUG"));
    m_logLevelMap.insert(std::make_pair(LOG_TRACE, "TRACE"));

    // read configuration file
    if (m_iniFile.loadFile(DEF_CONF_FILE_NAME) != G_YES)
    {
        setError("load file '%s' failed", DEF_CONF_FILE_NAME);
        return G_NO;
    }
    
    return G_YES;
}

GResult Logger::uninit()
{
    return G_YES;
}

void Logger::printLog(const LogLevel logLevel, 
    const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* format,
    va_list vaList)
{   
    ModuleRule* moduleRule = NULL;
    if (findModuleRule(std::string(module), moduleRule) == G_NO)
    {
        return;
    }

    if (logLevel > moduleRule->getLogLevel())
    {
        return;
    }

    GInt8 printBuf[DEF_ONE_LINE_BUF_SIZE] = {0};
    GUint64 pos = 0;

    // add log level and module name
    pos += sprintf(printBuf + pos, "<%s>[%s]", m_logLevelMap[logLevel], module);
    
    switch (moduleRule->getPrintLevel())
    {
    case PRINT_BASIC:
        break;
    case PRINT_MORE:
        // add file and function
        pos += sprintf(printBuf + pos, "(%s:%s)", file, function);
        break;
    case PRINT_FULL:
        // add file, line number and function
        pos += sprintf(printBuf + pos, "(%s:%s:%d)", file, function, line);    
        break;
    default:
        return;
    }

    // add log content
    pos += vsnprintf(printBuf + pos, DEF_ONE_LINE_BUF_SIZE - pos, format, vaList);

    // add word wrap
    if (m_globalRule.isWordWrap())
    {
        pos += sprintf(printBuf + pos, "\n");    
    }
    
    printf("%s", printBuf);
}

GInt8* Logger::getError()
{
    return m_error;
}

GResult Logger::findModuleRule(const std::string& moduleName, ModuleRule*& moduleRule)
{
    ModuleRuleMap::iterator iter = m_moduleRuleMap.find(moduleName); 
    if (iter == m_moduleRuleMap.end())
    {
        return G_NO;
    }

    moduleRule = iter->second;

    return G_YES;
}

void Logger::setError(const GInt8* args, ...)
{
    va_list vaList;
	va_start(vaList, args);    
	vsnprintf(m_error, G_ERROR_BUF_SIZE, args, vaList);
    va_end(vaList);	
}

GlobalRule::GlobalRule() : m_fileSize(0), m_fileCount(0), m_isWordWrap(true)
{
}

GlobalRule::~GlobalRule()
{
}

void GlobalRule::setFileName(const std::string& fileName)
{
    m_fileName = fileName;    
}

const std::string GlobalRule::getFileName() const
{
    return m_fileName;    
}

void GlobalRule::setFileSize(const GUint64 fileSize)
{ 
    m_fileSize = fileSize;   
}

GUint64 GlobalRule::getFileSize() const
{
    return m_fileSize;
}

void GlobalRule::setFileCount(const GUint32 fileCount)
{
    m_fileCount = fileCount;
}

GUint32 GlobalRule::getFileCount() const
{
    return m_fileCount;
}

void GlobalRule::setWordWrap(const bool isWordWrap)
{
    m_isWordWrap = isWordWrap;
}

bool GlobalRule::isWordWrap() const
{
    return m_isWordWrap;   
}
    
ModuleRule::ModuleRule() : m_logLevel(LOG_ERROR), m_printLevel(PRINT_BASIC)
{
}

ModuleRule::~ModuleRule()
{
}

void ModuleRule::setModuleName(const std::string& moduleName)
{
    m_moduleName = moduleName;
}

const std::string& ModuleRule::getModuleName() const
{
    return m_moduleName;
}

void ModuleRule::setLogLevel(const LogLevel logLevel)
{
    m_logLevel = logLevel;
}

const LogLevel& ModuleRule::getLogLevel() const
{
    return m_logLevel;
}

void ModuleRule::setPrintLevel(const PrintLevel printLevel)
{
    m_printLevel = printLevel;
}

const PrintLevel& ModuleRule::getPrintLevel() const
{
    return m_printLevel;
}
