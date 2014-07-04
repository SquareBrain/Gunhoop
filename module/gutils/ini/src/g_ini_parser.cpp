/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini_parser.cpp
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
#include <g_file.h>
#include <g_ini_parser.h>

G_NS_GUTILS_BEG

IniParser::IniParser()
{
}

IniParser::IniParser(const std::string& filePath)
{
    loadFile(m_filePath);
}

IniParser::~IniParser()
{
}

GResult IniParser::loadFile(const std::string& filePath)
{
    if (m_filePath == filePath)
    {
        return G_YES;
    }

    if (!m_filePath.empty())
    {
        cleanIniSectionMap();    
    }
    
    m_filePath = filePath;

    GCommon::File file(m_filePath.c_str());
    if (file.openR() != G_YES)
    {
        return G_NO;
    }

    GInt64 fileSize = file.getFileSize();
    if (fileSize <= 0)
    {   
        return G_NO;
    }
    
    GInt8* buffer = new GInt8[fileSize + 1];
    GInt64 readSize = file.readFile(buffer, fileSize);
    if (readSize != fileSize)
    {
        return G_NO;    
    }

    buffer[readSize] = 0;
    GResult ret = importData(buffer, fileSize);

    delete [] buffer;
    
    return ret;
}

GResult IniParser::importData(const std::string& fileData)
{
    return importData(fileData.c_str(), fileData.length());
}

GResult IniParser::importData(const GInt8* data, const GUint64 length)
{
    GUint64 offset = 0;
    
    while (length--)
    {
        switch ()
        {
        case '[':
            parserSection(data + offset, length);
            break;
        default:
            break;
        }

        offset++;
    }
    
    return G_YES;
}

GResult IniParser::getParaVal(const std::string& section, 
    const std::string paraName, 
    std::string& value)
{
    return G_YES;
}

GResult IniParser::setParaVal(const std::string& section, 
    const std::string paraName, 
    const std::string& value)
{
    return G_YES;    
}

GResult IniParser::saveFile() const
{
    return G_YES;
}

void IniParser::cleanIniSectionMap()
{
    GCommon::AutoLock autoLock(m_mapLock); 
    
    IniSectionMap::iterator iter = m_iniSectionMap.begin();
    while (iter != m_iniSectionMap.end())
    {
        m_iniSectionMap.erase(iter++);   
    }
}

GResult IniParser::parserSection(const GInt8* data, const GUint64& length, )
{
    GUint64 begPos = 0;
    GUint64 endPos = 0;
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

    std::string sectionName(data + endPos, endPos - begPos);
    IniSection* section = new IniSection(sectionName);

    getOneLine();
    
    offset = endPos;

    
}

G_NS_END
