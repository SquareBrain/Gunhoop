/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
* @note 
*
*  2. 2014-06-26 duye removal to gohoop project
*  1. 2013-11-15 duye Created this file
* 
*/
#include <stdarg.h>
#include <g_ini_section.h>
#include <g_ini.h>

static const GUint64 INI_TMP_BUF_SIZE = 1024 * 10;

GIniFile::GIniFile()
{
    m_error[0] = 0;
}

GIniFile::GIniFile(const std::string& filePath)
{
    loadFile(m_filePath);
}

GIniFile::~GIniFile()
{
}

GResult GIniFile::loadFile(const std::string& filePath)
{
    if (m_filePath == filePath)
    {
        setError("file has load");
        return G_NO;
    }

    if (!m_filePath.empty())
    {
        cleanIniSectionMap();    
    }
    
    m_filePath = filePath;

    GFile file(m_filePath.c_str());
    if (file.openFile(ONLY_READ) != G_YES)
    {
        setError("open file failed");
        return G_NO;
    }

    GInt64 fileSize = file.getFileSize();
    if (fileSize <= 0)
    {   
        file.closeFile();
        setError("file is empty");
        return G_NO;
    }
    
    GInt8* buffer = new GInt8[fileSize + 1];
    GInt64 readSize = file.readFile(buffer, fileSize);
    file.closeFile();
    
    if (readSize != fileSize)
    {
        setError("read file failed");
        return G_NO;    
    }

    buffer[readSize] = 0;
    GResult ret = importData(buffer, fileSize);

    delete [] buffer;
    buffer = NULL;
    
    return ret;
}

GResult GIniFile::importData(const std::string& fileData)
{
    return importData(fileData.c_str(), fileData.length());
}

GResult GIniFile::importData(const GInt8* data, const GUint64 length)
{
    GUint64 offset = 0;
    
    while (offset < length)
    {
        switch (data[offset])
        {
        case '[':
            if (parserSection(data, length, offset) != G_YES)
            {
                setError("file section failed");
                return G_NO;
            }
            break;
        default:
            offset++;
            break;
        }
    }
    
    return G_YES;
}

GResult GIniFile::getParaVal(const std::string& section, 
    const std::string& paraName, 
    std::string& value)
{
    GAutoLock autoLock(m_mapMutex); 
    GIniSectionMap::iterator iter = m_iniSectionMap.find(section);
    if (iter == m_iniSectionMap.end())
    {
        setError("section failed");
        return G_NO;
    }

    return iter->second->getPara(paraName, value);
}

GResult GIniFile::setParaVal(const std::string& section, 
    const std::string& paraName, 
    const std::string& value)
{
    GAutoLock autoLock(m_mapMutex); 
    GIniSectionMap::iterator iter = m_iniSectionMap.find(section);
    if (iter == m_iniSectionMap.end())
    {
        return G_NO;
    }

    return iter->second->setPara(paraName, value);    
}

GResult GIniFile::delSection(const std::string& section)
{
    GAutoLock autoLock(m_mapMutex); 
    GIniSectionMap::iterator iter = m_iniSectionMap.find(section);
    if (iter == m_iniSectionMap.end())
    {
        return G_NO;
    }

    m_iniSectionMap.erase(iter);

    return G_YES;    
}

GResult GIniFile::delPara(const std::string& section, const std::string& paraName)
{
    GAutoLock autoLock(m_mapMutex); 
    GIniSectionMap::iterator iter = m_iniSectionMap.find(section);
    if (iter == m_iniSectionMap.end())
    {
        return G_NO;
    }

    return iter->second->delPara(paraName);     
}

GResult GIniFile::saveFile()
{
    if (m_filePath.empty())
    {
        return G_NO;
    }

    saveFile(m_filePath);
    
    return G_YES;
}

GResult GIniFile::saveFile(const std::string& filePath)
{
    if (filePath.empty())
    {
        return G_NO;
    }

    GInt8 tmpBuf[INI_TMP_BUF_SIZE] = {0};
    GUint64 bufPos = 0;

    GAutoLock autoLock(m_mapMutex); 
    
    GIniSectionMap::iterator iter = m_iniSectionMap.begin();
    for (; iter != m_iniSectionMap.end(); ++iter)
    {
        bufPos += sprintf(tmpBuf + bufPos, "[%s]\n", iter->first.c_str());

        if (bufPos >= INI_TMP_BUF_SIZE)
        {
            return G_NO;
        }       
        
        GIniSection* section = iter->second;
        const GIniSection::KeyValueMap& keyValueMap = section->getkeyValueMap();
        GIniSection::KeyValueMap::const_iterator iter = keyValueMap.begin();
        for (; iter != keyValueMap.end(); ++iter)
        {
            bufPos += sprintf(tmpBuf + bufPos, "%s=%s\n", iter->first.c_str(), iter->second.c_str()); 
            if (bufPos >= INI_TMP_BUF_SIZE)
            {
                return G_NO;
            }
        }
    }

    tmpBuf[bufPos] = 0;

    GFile file(m_filePath.c_str());
    if (file.openFile(ONLY_WRITE) != G_YES)
    {
        return G_NO;
    }    

    GUint64 writeBytes = file.writeFile(tmpBuf, bufPos);
    file.closeFile();
    
    if (writeBytes != bufPos)
    {
        return G_NO;   
    }
    
    return G_YES;
}

GInt8* GIniFile::getError()
{
    return m_error;
}

void GIniFile::cleanIniSectionMap()
{
    GAutoLock autoLock(m_mapMutex); 
    
    GIniSectionMap::iterator iter = m_iniSectionMap.begin();
    while (iter != m_iniSectionMap.end())
    {
        delete iter->second;
        m_iniSectionMap.erase(iter++);   
    }
}

GResult GIniFile::parserSection(const GInt8* data, 
    const GUint64 length, 
    GUint64& offset)
{
    GUint64 begPos = offset;
    GUint64 endPos = offset;
    bool findSectionEnd = false;
    
    while (data[endPos] != 0)
    {
        switch (data[endPos++])
        {
        case '\r':
        case '\n':
        case ';':
        case '=':
        case ':':
            offset = endPos;
            return G_NO;
        case ']':
            findSectionEnd = true;
            break;
        default:
            break;
        }  

        if (findSectionEnd)
        {
            break;
        }
    }

    if (endPos - begPos <= 1)
    {
        offset = endPos;
        return G_NO;    
    }

    std::string sectionName(data + begPos, endPos - begPos);
    GIniSection* section = new GIniSection(sectionName);

    std::string lineStr;
    while (getOneLine(data + endPos, length - offset, lineStr) == G_YES)
    {
        offset += lineStr.length();
        GInt32 midPos = 0;
        if ((midPos = lineStr.find('=')) == std::string::npos)
        {
            continue;       
        }
        
        section->addPara(lineStr.substr(0, midPos), lineStr.substr(midPos + 1));
    }
    
    m_iniSectionMap.insert(std::make_pair(sectionName, section));
    
    offset = endPos;

    return G_YES;
}

GResult GIniFile::getOneLine(const GInt8* data, 
    const GUint64 length, 
    std::string& lineStr)
{
    if (length == 0)
    {
        return G_NO;
    }
    
    GUint64 endPos = 0;
    while (endPos < length)
    {
        if (data[endPos++] == '\n')
        {
            break;
        }
    }

    lineStr.assign(data, endPos);

    return G_YES;
}

void GIniFile::setError(const char *args,...)
{
    va_list vaList;
	va_start(vaList, args);    
	vsnprintf(m_error, G_ERROR_BUF_SIZE, args, vaList);
    va_end(vaList);	
}
