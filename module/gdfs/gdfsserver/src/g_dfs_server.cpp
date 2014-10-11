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

DfsServer::~DfsServer() 
{
    gcom::ServerFactory::intance().destroyServer(m_httpServer);
    gcom::ServerFactory::intance().destroyServer(m_ftpServer);
    gcom::ServerFactory::intance().destroyServer(m_cliServer);
}

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
    
    // init all service
    m_httpServer = gcom::ServerFactory::intance().createServer(HTTP_SERVER);
    m_ftpServer = gcom::ServerFactory::intance().createServer(FTP_SERVER);
    m_cliServer = gcom::ServerFactory::intance().createServer(CLI_SERVER);
    
    // start routine() thread
    this->startTask();
        
    // set server state is work
    m_serverState = HOST_SERVER_WORK;
    
    G_LOG_OUT();
    
    return G_YES;
}

GResult DfsServer::stop()
{
    G_LOG_IN();
    
    m_httpServer->stop();
    m_ftpServer->stop();
    m_cliServer->stop();
    
    G_LOG_OUT();
    
    return G_NO;
}

DfsServerState DfsServer::routine()
{
    G_LOG_IN();
    
    m_httpServer->start();
    m_ftpServer->start();
    m_cliServer->start();
    
    for (;;)
    {   
        // startup all server
        gcom::NetworkServerMonitor::keepServer(m_httpServer);
        gcom::NetworkServerMonitor::keepServer(m_ftpServer);
        gcom::NetworkServerMonitor::keepServer(m_cliServer);
        
        gsys::System::sleep(5);    
    }
    
    G_LOG_OUT();
    
    return G_YES;
}

}
