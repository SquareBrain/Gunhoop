/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_dfs_server.h
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

#include <g_dfs_http_server.h>
#include <g_dfs_ftp_server.h>
#include <g_dfs_cli_server.h>

static const std::string DEF_DFS_CFG_FILE_PATH("./dfs_cfg.xml");

namespace gdfs {
    
/**
 * @brief server state
 */
typedef enum
{
    SERVER_INIT,
    SERVER_FAILED,
    SERVER_RUNNING,
    SERVER_STOP
} DFSServerState;

/**
 * @brief dfs server
 */
class DFSServer : public gsys::ThreadTask, public gcom::Singleton<DFSServer>
{
public:
    DFSServer();
    explicit DFSServer(const std::string& dfs_cfg_file_path);
    ~DFSServer();
    
    /**
     * @brief startup DFF service
     */
    GResult start();
    
    /**
     * @brief stop DFS service
     */
    GResult stop();
    
    /**
     * @brief set/get DFS service state
     */
    void setState(const DFSServerState state);
    const DFSServerState& getState() const;
    
private:
    // inherit from base class gsys::ThreadTask
    GResult run();
    // read DFS Server configuration file
    GResult loadCfg();
    // create service thread
    GResult createService();
    
private:
    DFSServerState      m_serverState;  
    std::string         m_dfsCfgFilePath;
    gcom::CfgMgr        m_cfgMgr;
    
    // dfs services list
    DFSHttpServer       m_httpServer;
    DFSFtpServer        m_ftpServer;
    DFSCliServer        m_cliServer;
};
}
