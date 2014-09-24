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

#include <g_tcp.h>
#include <g_udp.h>
#include <g_rpc.h>
#include <g_http.h>
#include <g_thread.h>

/**
 * @brief dfs server
 */
 
static const std::string DEF_DFS_CFG_FILE("./dfs_cfg.xml");

namespace gdfs {
 
class DFSServer : public ThreadTask
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
    
private:
    GResult run();
    
private:
    
};
 
}
