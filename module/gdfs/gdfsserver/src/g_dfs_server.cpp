/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_dfs_server.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-09-24
* @note 
*
*  1. 2014-09-24 duye Created this file
* 
*/

#include <g_dfs_server.h>
#include <g_logger.h>

static const GInt8* LOG_PREFIX = "dfs.server.startup";

namespace gdfs {

DfsServer::DfsServer() : gcom::HostServer() {}

DfsServer::DfsServer(const std::string& dfs_cfg_file_path) : gcom::HostServer(), m_dfsCfgFilePath(dfs_cfg_file_path) {}

DfsServer::~DfsServer() {}

GResult DfsServer::start()
{
    G_LOG_IN();
    
    if (getServerState() == HOST_SERVER_WORK)
    {
        return G_YES;
    }
    
    if (m_dfsCfgFilePath.empty())
    {
        m_dfsCfgFilePath = DEF_DFS_CFG_FILE_PATH;
    }
    
    IS_NO_R(m_cfgMgr.load(m_dfsCfgFilePath));
    IS_NO_R(startupService());
    
    m_serverState = HOST_SERVER_WORK;
    
    G_LOG_OUT();
    
    return G_YES;
}

GResult DfsServer::stop()
{
    G_LOG_IN();
    G_LOG_OUT();
    
    return G_NO;
}

DfsServerState DfsServer::routine()
{
    G_LOG_IN();
    G_LOG_OUT();
    
    return G_YES;
}

GResult DfsServer::startupService()
{
    G_LOG_IN();
    G_LOG_OUT();
    
    return G_YES;
}

}
