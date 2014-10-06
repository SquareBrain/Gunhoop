/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_cfg_mgr.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-09-28
* @note 
*
*  1. 2014-09-28 duye Created this file
*/

#include <g_logger.h>
#include <g_cfg_mgr.h>

namespace gcom {

static const GInt8* LOG_PREFIX = "gcom.function.cfgmgr";

CfgMgr::CfgMgr() {}
CfgMgr::CfgMgr(const std::string& cfg_file_path) : m_cfgFilePath(cfg_file_path) {}
CfgMgr::~CfgMgr() 
{

}

GResult CfgMgr::load()
{
    if (m_cfgFilePath.empty())
    {
        G_LOG_ERROR(LOG_PREFIX, "configuration file path is empty");
        return G_NO;
    }
    
    return load(m_cfgFilePath);
}

GResult CfgMgr::load(const std::string& cfg_file_path)
{
    if (!m_cfgDoc.loadFile(cfg_file_path))
    {
        G_LOG_ERROR(LOG_PREFIX, "load configuration file %s failed", m_cfgFilePath.c_str());
        return G_NO;        
    }    
    
    return G_YES;
}

GResult CfgMgr::loadData(const GInt8* cfg_data)
{
    if (cfg_data == nullptr)
    {
        G_LOG_ERROR(LOG_PREFIX, "cfg_data == nullptr");
        return G_NO;                
    }
    
    return G_YES;
}

GResult CfgMgr::loadData(const std::string cfg_data)
{
    return loadData(cfg_data.c_str());
}

GResult CfgMgr::setValue(const std::string& path, const bool value)
{
    return G_YES;
}

GResult CfgMgr::setValue(const std::string& path, const GInt32 value)
{
    return G_YES;
}

GResult CfgMgr::setValue(const std::string& path, const GInt64 value)
{
    return G_YES;
}

GResult CfgMgr::setValue(const std::string& path, const GUint32 value)
{
    return G_YES;
}

GResult CfgMgr::setValue(const std::string& path, const GUint64 value)
{
    return G_YES;
}

GResult CfgMgr::setValue(const std::string& path, const GInt8* value)
{
    return G_YES;
}

GResult CfgMgr::setValue(const std::string& path, const std::string& value)
{
    return G_YES;
}

GResult CfgMgr::getValue(const std::string& path, bool& value)
{
    return G_YES;
}

GResult CfgMgr::getValue(const std::string& path, GInt32& value)
{
    return G_YES;
}

GResult CfgMgr::getValue(const std::string& path, GInt64& value)
{
    return G_YES;
}

GResult CfgMgr::getValue(const std::string& path, GUint32& value)
{
    return G_YES;
}

GResult CfgMgr::getValue(const std::string& path, GUint64& value)
{
    return G_YES;
}

GResult CfgMgr::getValue(const std::string& path, std::string& value)
{
    return G_YES;
}

GResult CfgMgr::getValue(const std::string& path, GInt8* buffer, const GUint64 size)
{
    return G_YES;
}

GResult CfgMgr::save()
{
    return G_YES;
}
}
