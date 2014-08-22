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
    
GModuleRule::GModuleRule() 
    : m_logLevel(LOG_ERROR)
    , m_printFormat(PRINT_BASIC)
    , m_saveWay(SAVE_STDOUT)
{
}

GModuleRule::~GModuleRule()
{
}

void GModuleRule::setModuleName(const std::string& moduleName)
{
	m_moduleName = moduleName;
}

const std::string& GModuleRule::getModuleName() const
{
	return m_moduleName;
}

void GModuleRule::setLogLevel(const GLogLevel& logLevel)
{
	m_logLevel = logLevel;
}

const GLogLevel& GModuleRule::getLogLevel() const
{
	return m_logLevel;
}

void GModuleRule::setPrintFormat(const GPrintFormat& printFormat)
{
	m_printFormat = printFormat;
}

const GPrintFormat& GModuleRule::getPrintFormat() const
{
	return m_printFormat;
}

void GModuleRule::setSaveWay(const GSaveWay& saveWay)
{
	m_saveWay = saveWay;
}

const GSaveWay& GModuleRule::getSaveWay() const
{
	return m_saveWay;
}

void GModuleRule::setFilePrefix(const std::string& filePrefix)
{
	m_filePrefix = filePrefix;
}

const std::string& GModuleRule::getFilePrefix() const
{
	return m_filePrefix;
}

void GModuleRule::setFilePath(const std::string& filePath)
{
	m_filePath = filePath;
}

const std::string& GModuleRule::getFilePath() const
{
	return m_filePath;
}

GLogFile::GLogFile(const std::string& fileName, const GUint64 maxFileSize) 
	: m_file(nullptr)
	, m_fileName(fileName) 
	, m_maxFileSize(maxFileSize)
	, m_currFileSize(0)
	, m_genFileCount(0)
{
}

GLogFile::~GLogFile()
{
	delete m_file;
}

GResult GLogFile::write(const GInt8* data, const GUint64 len)
{
	if (m_file == nullptr)
	{
		if (open() != G_YES)
		{
			return G_NO;	
		}
	}
	
	if (m_file->write(data, len) < 0)
	{
		return G_NO;
	}
	
	m_currFileSize += len;
	
	if (m_currFileSize >= m_maxFileSize)
	{
		open();
	}
	
	return G_YES;
}

GResult GLogFile::open()
{
	std::string fileFullName;
	
	while (true)
	{
		m_genFileCount++;
		
		fileFullName = m_fileName + "_" + std::to_string(m_genFileCount) + ".glog";
		
		if (GFileUtil::isExist(fileFullName.c_str()))
		{
			m_genFileCount++;
			continue;
		}
		
		break;
	}
	
	delete m_file;
	m_file = new GFile(fileFullName.c_str());
	if (m_file->open(G_OPEN_WRITE) != G_YES)
	{
		return G_NO;
	}	
	
	return G_YES;
}

GLoggerImpl::GLoggerImpl()
{
	m_logLevelMap.insert(std::make_pair(LOG_ERROR, "NULL"));
	m_logLevelMap.insert(std::make_pair(LOG_ERROR, "ERROR"));
	m_logLevelMap.insert(std::make_pair(LOG_WARN, "WARN"));
	m_logLevelMap.insert(std::make_pair(LOG_INFO, "INFO"));
	m_logLevelMap.insert(std::make_pair(LOG_DEBUG, "DEBUG"));
	m_logLevelMap.insert(std::make_pair(LOG_TRACE, "TRACE"));
	
	m_printFormatMap.insert(std::make_pair(PRINT_BASIC, "basic"));
	m_printFormatMap.insert(std::make_pair(PRINT_MORE, "more"));
	m_printFormatMap.insert(std::make_pair(PRINT_FULL, "full"));
	
	m_saveWayMap.insert(std::make_pair(SAVE_STDOUT, "stdout"));
	m_saveWayMap.insert(std::make_pair(SAVE_STDERR, "stderr"));
	m_saveWayMap.insert(std::make_pair(SAVE_FILE, "file"));
	
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
	// parser file
	if (parserLogConf() != G_YES)
	{
		setError("parser log conf file '%s' failed", DEF_CONF_FILE_NAME);
		return G_NO;
	}
	
	// init file
	if (initFile() != G_YES)
	{
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
	GModuleRule* moduleRule = findModuleRule(module);
	if (moduleRule == nullptr)
	{
		return;
	}

	if (logLevel > moduleRule->getLogLevel() || logLevel > m_globalRule.getTopLogLevel())
	{
		return;
	}

	GInt8 printBuf[DEF_ONE_LINE_BUF_SIZE] = {0};
	GUint64 pos = 0;
	
	if (moduleRule->getPrintFormat() >= PRINT_MORE)
	{
		// add time
		GInt8 timeStr[128] = {0};
        
		pos += sprintf(printBuf + pos, "%s", 
            GCommon::TimeConv::convTime("ye-mo-da ho:mi:se:ms", timeStr, ));   
	}

	// add log level and module name
	pos += sprintf(printBuf + pos, "<%s>[%s]", m_logLevelMap[logLevel].c_str(), module);

	switch (moduleRule->getPrintFormat())
	{
	case PRINT_BASIC:
		break;
	case PRINT_MORE:
		// add time
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
			if (text == nullptr)
			{
				// set default value
				m_globalRule.setTopLogLevel(LOG_INFO);	
				continue;
			}
			
			GLogLevel logLevel = LOG_NULL;
			if (findLogLevel(text, logLevel) != G_YES)
			{
				setError("parser log configuration error for get <toplevel>, please check file '%s'", DEF_CONF_FILE_NAME);
				return G_NO;
			}
			
			m_globalRule.setTopLogLevel(logLevel);
		}
		else if (childElement->valueStr() == "maxfilenum")
		{
			const GInt8* text = childElement->getText();
			if (text == nullptr)
			{
				// set default value
				m_globalRule.setMaxFileNum(10);	
				continue;
			}
			
			GInt32 maxFileNum = std::stoi(text);
			if (maxFileNum <= 0)
			{
				setError("parser log configuration error for get <maxfilenum>, please check file '%s'", DEF_CONF_FILE_NAME);
				return G_NO;					
			}
			
			m_globalRule.setTopLogLevel(logLevel);
		}
		else if (childElement->valueStr() == "maxfilesize")
		{
			const GInt8* text = childElement->getText();
			if (text == nullptr)
			{
				m_globalRule.setMaxFileSize(10);
				continue;
			}
			
			GInt32 maxFileSize = std::stoi(text);
			if (maxFileSize <= 0)
			{
				setError("parser log configuration error for get <maxfilesize>, please check file '%s'", DEF_CONF_FILE_NAME);
				return G_NO;
			}
			
			m_globalRule.setMaxFileSize(maxFileSize);
		}
		else if (childElement->valueStr() == "autowordwrap")
		{
			const GInt8* text = childElement->getText();
			if (strcmp(text, "true") == 0)
			{
				m_globalRule.setAutoWordwrap(true);	
			}
			else if (strcmp(text, "false") == 0)
			{
				m_globalRule.setAutoWorkwrap(false);
			}
			else
			{
				setError("parser log configuration error for get <autowordwrap>, please check file '%s", DEF_CONF_FILE_NAME);
				return G_NO;
			}
		}
		else if (childElement->valueStr() == "module")
		{
			// get module rule
			GInt8* moduleName = childElement->attribute("name");
			if (moduleName == nullptr)
			{
				setError("parser log configuration error for get <module name>, please check file '%s'", DEF_CONF_FILE_NAME);
				return G_NO;
			}
			
			if (m_moduleRuleMap.find(moduleName) != m_moduleRuleMap.end())
			{
				continue;
			}
			
			GInt8* levelStr = childElement->attribute("level");
			if (level == nullptr)
			{
				setError("parser log configuration error for get <module level>, please check file '%s'", DEF_CONF_FILE_NAME)
				return G_NO;
			}
			
			GLogLevel logLevel = LOG_NULL;
			if (findLogLevel(levelStr, logLevel) < 0)
			{
				setError("parser log configuration error for get <module level>, please check file '%s", DEF_CONF_FILE_NAME);
				return G_NO;
			}
			
			GInt8* formatStr = childElement->attribute("format");
			if (format == nullptr)
			{
				setError("parser log configuration error for get <module format>, please check file '%s", DEF_CONF_FILE_NAME);
				return G_NO;
			}
			
			GPrintFormat printFormat = PRINT_BASIC;
			if (findPrintFormat(formatStr, printFormat) < 0)
			{
				setError("parser log configuration error for get <module format>, please check file '%s", DEF_CONF_FILE_NAME);
				return G_NO;
			}
			
			GInt8* outo = childElement->attribute("outo");
			if (outo == nullptr)
			{
				setError("parser log configuration error for get <module outo>, please check file '%s", DEF_CONF_FILE_NAME);
				return G_NO;
			}
			
			GSaveWay saveWay = SAVE_STDOUT;
			if (findSaveWay(outo, saveWay) < 0)
			{
				setError("parser log configuration error for get <module outo>, please check file '%s", DEF_CONF_FILE_NAME);
				return G_NO;
			}
			
			GModuleRule* moduleRule = new GModuleRule;
			moduleRule->setModuleName(name);
			moduleRule->setLogLevel(logLevel);
			moduleRule->setPrintFormat(printFormat);
			moduleRule->setSaveWay(saveWay);
			
			GInt8* prefix = childElement->attribute("prefix");
			if (prefix != nullptr)
			{
				moduleRule->setFilePrefix(prefix);
			}
			
			GInt8* path = childElement->attribute("path");
			if (path != nullptr)
			{
				moduleRule->setFilePath(path);
			}
			
			m_moduleRuleMap.insert(std::make_pair(moduleName, moduleRule));
		}        
	}
	
	return G_YES;	
}

GResult GLoggerImpl::initFile()
{
	GModuleRuleMap::iterator iter = m_moduleRuleMap.begin();
	for (; iter 1= m_moduleRuleMap.end(); ++iter)
	{
		GModuleRule* moduleRule = iter->second;
		if (moduleRule->getSaveWay() != SAVE_FILE)
		{
			continue;
		}
		
		std::string filePath = moduleRule->getFilePath();
		if (filePath == "." || filePath.empty())
		{
			filePath = "./";
		}
		else if (filePath.at(filePath.length() - 1) != '/')
		{
			filePath.append('/');
		}
		
		std::string fileName;
		if (moduleRule->getFilePrefix().empty())
		{
			fileName = filePath + moduleRule->getModuleName();
		}
		else
		{
			fileName = filePath + moduleRule->getFilePrefix();
		}
		
		moduleRule->setFileName(fileName);
		if (g_logFileMap.find(fileName) == g_logFileMap.end())
		{
			g_logFileMap.insert(std::make_pair(fileName, new GLogFile(fileName, g_globalRule->getMaxFileNum())));	
		}
	}
	
	return G_YES;
}

GResult GLoggerImpl::findLogLevel(const GInt8* logLevelStr, GLogLevel& logLevel)
{
	GLogLevelMap::iterator iter = m_logLevelMap.begin();
	for (; iter != m_logLevelMap.end(); ++iter)
	{
		if (iter->second->compare(logLevelStr) == 0)
		{
			logLevel = iter->first;
			return YES;
		}
	}
	
	return G_NO;	
}

GResult GLoggerImpl::findPrintFormat(const GInt8* printFormatStr, GPrintFormat& printFormat)
{
	GPrintFormatMap::iterator iter = m_printFormatMap.begin();
	for (; iter != m_printFormatMap.end(); ++iter)
	{
		if (iter->second->compare(printFormatStr) == 0)
		{
			logLevel = iter->first;
			return G_YES;
		}
	}
	
	return G_NO;	
}

GResult GLoggerImpl::findSaveWay(const GInt8* saveWayStr, GSaveWay& saveWay)
{
	GSaveWayMap::iterator iter = m_saveWayMap.begin();
	for (; iter != m_saveWayMap.end(); ++iter)
	{
		if (iter->second->compare(saveWayStr) == 0)
		{
			saveWay = iter->first;
			return G_YES;
		}
	}
}

const GModuleRule* GLoggerImpl::findModuleRule(const std::string& moduleName) const
{
	GModuleRuleMap::const_iterator iter = m_moduleRuleMap.find(moduleName); 
	if (iter == m_moduleRuleMap.end())
	{
		return NULL;
	}

	return iter->second;
}

void GLoggerImpl::setError(const GInt8* args, ...)
{
	GSys::format(m_error, G_ERROR_BUF_SIZE, args);
}
