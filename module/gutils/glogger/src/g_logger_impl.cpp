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
#include <g_file.h>
#include <g_xml.h>
#include <g_logger_impl.h>

// the max buffer size of in one line
static const GUint64 DEF_ONE_LINE_BUF_SIZE = 1024;

// the configuration file name
static const GInt8* DEF_CONF_FILE_NAME = "glog.xml";

GLoggerImpl::GLoggerImpl()
{
	m_error[0] = 0;
}

GLoggerImpl::~GLoggerImpl()
{
}

GLoggerImpl* GLoggerImpl::GetInstance()
{
	static GLoggerImpl glogger;
	return &glogger;
}

GResult GLoggerImpl::init()
{    
	m_logLevelMap.insert(std::make_pair(LOG_ERROR, "NULL"));
	m_logLevelMap.insert(std::make_pair(LOG_ERROR, "ERROR"));
	m_logLevelMap.insert(std::make_pair(LOG_WARN, "WARN"));
	m_logLevelMap.insert(std::make_pair(LOG_INFO, "INFO"));
	m_logLevelMap.insert(std::make_pair(LOG_DEBUG, "DEBUG"));
	m_logLevelMap.insert(std::make_pair(LOG_TRACE, "TRACE"));

	// parser file
	if (parserLogConf() != G_YES)
	{
		setError("parser log conf file '%s' failed", DEF_CONF_FILE_NAME);
		return G_NO;
	}

	return G_YES;
}

GResult GLoggerImpl::uninit()
{
	return G_YES;
}

void GLoggerImpl::printLog(const GLogLevel logLevel, 
	const GInt8* module, 
	const GInt8* file, 
	const GUint32 line, 
	const GInt8* function,
	const GInt8* args, ...)
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
	pos += sprintf(printBuf + pos, "<%s>[%s]", m_logLevelMap[logLevel].c_str(), module);

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
	pos += GSys::format(printBuf + pos, DEF_ONE_LINE_BUF_SIZE - pos, args);

	// add word wrap
	if (m_globalRule.isAutoWordWrap())
	{
		pos += sprintf(printBuf + pos, "\n");    
	}

	printf("%s", printBuf);
}

GInt8* GLoggerImpl::getError()
{
	return m_error;
}

GResult GLoggerImpl::parserLogConf()
{
	// read configuration file
	GXmlDocument logDoc;
	if (!logDoc.loadFile(DEF_CONF_FILE_NAME))
	{
	    setError("load log conf file '%s' failed", DEF_CONF_FILE_NAME);
	    return G_NO;
	}
	
	logDoc.print();
	
	GXmlElement* rootElement = logDoc.rootElement();
	GXmlElement* childElement = rootElement->firstChildElement();
	for (; childElement != nullptr; childElement = childElement->nextSiblingElement())
	{
		if (childElement->valueStr() == "toplevel")
		{
			const GInt8* text = childElement->getText();
			if (text != nullptr)
			{
				GLogLevel logLevel = LOG_NULL;
				if (findLogLevel(text, logLevel) != G_YES)
				{
					setError("log conf format error, please check glog.xml");
					return G_NO;
				}
				
				m_globalRule.setTopLogLevel(logLevel);
			}
		}
		else if (childElement->valueStr() == "maxfilenum")
		{
			
		}
		else if (childElement->valueStr() == "maxfilesize")
		{
			
		}
		else if (childElement->valueStr() == "autowordwrap")
		{
			// GXmlAttribute* attribute = childElement->firstAttribute();
		}
		else if (childElement->valueStr() == "module")
		{
			// GXmlAttribute* attribute = childElement->firstAttribute();
		}        
	}
	
	return G_YES;	
}

GResult GLoggerImpl::findLogLevel(const GInt8* logLevelStr, GLogLevel& logLevel)
{
	return G_YES;	
}

GResult GLoggerImpl::findModuleRule(const std::string& moduleName, ModuleRule*& moduleRule)
{
	GModuleRuleMap::iterator iter = m_moduleRuleMap.find(moduleName); 
	if (iter == m_moduleRuleMap.end())
	{
		return G_NO;
	}

	moduleRule = iter->second;

	return G_YES;
}

void GLoggerImpl::setError(const GInt8* args, ...)
{
	GSys::format(m_error, G_ERROR_BUF_SIZE, args);
}

GlobalRule::GlobalRule() 
    : m_topLogLevel(LOG_TRACE)
    , m_maxFileNum(0)
    , m_maxFileSize(0)
    , m_isAutoWordWrap(true)
{
}

GlobalRule::~GlobalRule()
{
}

void GlobalRule::setMaxFileNum(const GUint32 maxFileNum)
{
	m_maxFileNum = maxFileNum;
}

GUint32 GlobalRule::getMaxFileNum() const
{
	return m_maxFileNum;
}

void GlobalRule::setMaxFileSize(const GUint64 maxFileSize)
{ 
	m_maxFileSize = maxFileSize;   
}

GUint64 GlobalRule::getMaxFileSize() const
{
	return m_maxFileSize;
}

void GlobalRule::setAutoWordWrap(const bool isAutoWordWrap)
{
	m_isAutoWordWrap = isAutoWordWrap;
}

bool GlobalRule::isAutoWordWrap() const
{
	return m_isAutoWordWrap;   
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

void ModuleRule::setLogLevel(const GLogLevel& logLevel)
{
	m_logLevel = logLevel;
}

const GLogLevel& ModuleRule::getLogLevel() const
{
	return m_logLevel;
}

void ModuleRule::setPrintLevel(const GPrintLevel& printLevel)
{
	m_printLevel = printLevel;
}

const GPrintLevel& ModuleRule::getPrintLevel() const
{
	return m_printLevel;
}

void ModuleRule::setSaveWay(const GSaveWay& saveWay)
{
	m_saveWay = saveWay;
}

const GSaveWay& ModuleRule::getSaveWay() const
{
	return m_saveWay;
}

void ModuleRule::setFilePrefix(const std::string& filePrefix)
{
	m_filePrefix = filePrefix;
}

const std::string& ModuleRule::getFilePrefix() const
{
	return m_filePrefix;
}

void ModuleRule::setFilePath(const std::string& filePath)
{
	m_filePath = filePath;
}

const std::string& ModuleRule::getFilePath() const
{
	return m_filePath;
}
