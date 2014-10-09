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
#include <g_utils.h>

static const std::string DEF_DFS_CFG_FILE_PATH("./dfs_cfg.xml");

namespace gdfs {

/**
 * @brief dfs server
 */
class DfsServer : public gcom::HostServer
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
     * @brief stop service
     * @return G_YES/G_NO
     * @note inherit from base class gcom::HostServer
     */
    GResult stop();
	
private:	
    /**
     * @brief message loop handle
     * @return G_YES/G_NO
     * @note inherit from base class gcom::HostServer
     */
    GResult routine() = 0;     
    
    // read DFS Server configuration file
    GResult loadCfg();
    
    // create service thread
    GResult createService();
    
private:
    std::string             m_dfsCfgFilePath;
    gcom::CfgMgr            m_cfgMgr;
    gcom::NetworkServer*    m_httpServer;
    gcom::NetworkServer*    m_ftpServer;
    gcom::NetworkServer*    m_cliServer;
};
}
