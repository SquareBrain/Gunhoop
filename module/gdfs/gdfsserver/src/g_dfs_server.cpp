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

DfsServer::DfsServer(){}

DfsServer::DfsServer(const std::string& dfs_cfg_file_path) : m_dfsCfgFilePath(dfs_cfg_file_path) {}

DfsServer::~DfsServer() 
{
    G_LOG_IN();
    gcom::ServerFactory::intance().destroy(m_httpServer);
    gcom::ServerFactory::intance().destroy(m_ftpServer);
    gcom::ServerFactory::intance().destroy(m_cliServer);
    G_LOG_OUT();
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
    
    // read server configuration
    if (IS_NO(m_cfgMgr.load(m_dfsCfgFilePath)))
    {
        G_LOG_ERROR(LOG_PREFIX, "load configuration file '%s' failed", m_dfsCfgFilePath.c_str());
        return G_NO;
    }
    
    // init all service
    m_httpServer = gcom::ServerFactory::intance().create(HTTP_SERVER);
    m_ftpServer = gcom::ServerFactory::intance().create(FTP_SERVER);
    m_cliServer = gcom::ServerFactory::intance().create(CLI_SERVER);
    
    // startup all service
    m_httpServer->start();
    m_ftpServer->start();
    m_cliServer->start();    
    
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

void DfsServer::setCfgFilePath(const std::string& dfs_cfg_file_path)
{
    m_dfsCfgFilePath = dfs_cfg_file_path; 
}

DfsServerState DfsServer::routine()
{
    G_LOG_IN();
    
    for (;;)
    {   
        // keep all service is running
        gcom::NetworkServerMonitor::keepping(m_httpServer);
        gcom::NetworkServerMonitor::keepping(m_ftpServer);
        gcom::NetworkServerMonitor::keepping(m_cliServer);
        
        gsys::System::sleep(5);    
    }
    
    G_LOG_OUT();
    
    return G_YES;
}

}
