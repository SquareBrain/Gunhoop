/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_dfs_server.h
* @version     
* @brief      
* @author   duye
* @date     2014-09-24
* @note 
*
*  1. 2014-09-24 duye Created this file
* 
*/
#pragma once

#include <g_system.h>
#include <g_common.h>

static const std::string DEF_DFS_CFG_FILE_PATH("./dfs_cfg.xml");

namespace gdfs {

/**
 * @brief dfs server
 */
class DfsServer : public gcom::HostServer, public gcom::Singleton<DfsServer>
{
public:
    DfsServer();
    /**
     * @brief constructor
     * @param [in] dfs_cfg_file_path, dfs server configuration file
     */
    explicit DfsServer(const std::string& dfs_cfg_file_path);
    ~DfsServer();
    
    /**
     * @brief startup service
     * @return G_YES/G_NO
     * @note inherit from base class gcom::HostServer
     */       
    GResult start();
   
    /**
     * @brief startup service
     * @param [in] dfs_cfg_file_path, dfs server configuration file
     * @return G_YES/G_NO
     */       
    GResult start(const std::string& dfs_cfg_file_path);
    
    /**
     * @brief stop service
     * @return G_YES/G_NO
     * @note inherit from base class gcom::HostServer
     */
    GResult stop();
    
    /**
     * @brief set dfs server configuration file path
     * @param [in] dfs_cfg_file_path: file path
     */
    void setCfgFilePath(const std::string& dfs_cfg_file_path);
	
private:	
    // message loop handle, inherit from base class gcom::HostServer
    GResult routine();
    
private:
    std::string             m_dfsCfgFilePath;
    gcom::CfgMgr            m_cfgMgr;
    gcom::NetworkServer*    m_httpServer;
    gcom::NetworkServer*    m_ftpServer;
    gcom::NetworkServer*    m_cliServer;
};
}
