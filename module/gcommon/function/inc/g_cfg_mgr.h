/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_cfg_mgr.h
* @version     
* @brief      
* @author   duye
* @date     2014-09-28
* @note 
*
*  1. 2014-09-28 duye Created this file
* 
*/

#pragma once

#include <string>

#include <g_type.h>
#include <g_system.h>
#include <g_utils.h>

namespace gcom {

/**
 * @brief configuration manager class
 */
class CfgMgr
{
public:
    CfgMgr();
    explicit CfgMgr(const std::string& cfg_file_path);
    ~CfgMgr();
    
    /**
     * @breif load configuration
     * @return G_YES/G_NO
     */
    GResult load();
    GResult load(const std::string& cfg_file_path);
    GResult load(const GInt8* cfg_data);
    GResult load(const std::string cfg_data);
    
    /**
     * @brief set configuration value
     * @param [in] path : parameter path, like : dfscfg.server.port
     * @param [in] value : set value
     * @return G_YES/G_NO
     */
    GResult setValue(const std::string& path, const bool value);
    GResult setValue(const std::string& path, const GInt32 value);
    GResult setValue(const std::string& path, const GInt64 value);
    GResult setValue(const std::string& path, const GUInt32 value);
    GResult setValue(const std::string& path, const GUInt64 value);
    GResult setValue(const std::string& path, const GInt8* value);
    GResult setValue(const std::string& path, const std::string& value);
    
    /**
     * @brief get configuration value
     * @param [in] path : parameter path, like : dfscfg.server.port
     * @param [out] value : get value
     * @return G_YES/G_NO
     */
    GResult getValue(const std::string& path, bool& value);
    GResult getValue(const std::string& path, GInt32& value);
    GResult getValue(const std::string& path, GInt64& value);
    GResult getValue(const std::string& path, GUint32& value);
    GResult getValue(const std::string& path, GUint64& value);
    GResult getValue(const std::string& path, std::string& value);
    
    /**
     * @brief get configuration data
     * @param [in] path : parameter path, like : dfscfg.server.port
     * @param [out] buffer : data buffer
     * @param [in] size : data buffer size
     * @return G_YES/G_NO
     */    
    GResult getValue(const std::string& path, GInt8* buffer, const GUint64 size);
    
    /**
     * @brief save configuration 
     * @return G_YES/G_NO
     */
    GResult save();
    
private:
    GResult loadCfg();
    GResult loadCfg(const GInt8* cfg_data);
    
private:
    std::string     m_cfgFilePath;
    XmlDocument     m_cfgDoc;
};
}
