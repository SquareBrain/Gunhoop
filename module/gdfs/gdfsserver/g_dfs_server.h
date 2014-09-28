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

using namespace gcom;
using namespace gsys;

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
class DFSServer : public ThreadTask, public Singleton
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
    DFSServerState getState();
    
private:
    GResult run();
    GResult loadCfg();
    GResult createService();
    
private:
    DFSServerState  m_serverState;  
    XmlDocument     m_dfsCfgFile;
};
 
}
