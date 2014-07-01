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
#include <stdio.h>
#include <g_logger_impl.h>

static const GUint64 DEF_ONE_LINE_BUF_SIZE = 1024;

G_NS_GUTILS_BEG
  
GResult LoggerUtil::init()
{
    return Logger::GetInstance()->init();
}

GResult LoggerUtil::uninit()
{
    return Logger::GetInstance()->uninit();
}

void LoggerUtil::printError(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* log)
{
    Logger::GetInstance()->printLog(LOG_ERROR, );    
}

void LoggerUtil::printWarn(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* log)
{
}

void LoggerUtil::printInfo(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* log)
{
}

void LoggerUtil::printDebug(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* log)
{
}

void LoggerUtil::printTrace(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* log)
{
}

Logger::Logger()
{
}

Logger::~Logger()
{
}

GResult Logger::init()
{    
    m_logLevelMap.insert(std::make_pair(LOG_ERROR, "ERROR"));
    m_logLevelMap.insert(std::make_pair(LOG_WARN, "WARN"));
    m_logLevelMap.insert(std::make_pair(LOG_INFO, "INFO"));
    m_logLevelMap.insert(std::make_pair(LOG_DEBUG, "DEBUG"));
    m_logLevelMap.insert(std::make_pair(LOG_TRACE, "TRACE"));
    
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
    const GInt8* log)
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
    pos += sprintf(printBuf + pos, "%s", log);

    // add word wrap
    if (m_globalRule->isWordWrap())
    {
        pos += sprintf(printBuf + pos, "\n", log);    
    }
    
    printf("%s", printBuf);
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
    m_levelName.assign("");
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

G_NS_END
