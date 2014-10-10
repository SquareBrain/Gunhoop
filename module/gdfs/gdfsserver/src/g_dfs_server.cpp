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
    DfsServerFactory::DestroyServer(HTTP_SERVER, m_httpServer);
    DfsServerFactory::DestroyServer(FTP_SERVER, m_httpServer);
    DfsServerFactory::DestroyServer(CLI_SERVER, m_httpServer);
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
    IS_NO_R(initService());
    
    m_serverState = HOST_SERVER_WORK;
    
    // start routine() thread
    this->startTask();
    
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
        if (m_httpServer->state() != SERVER_WORK)
        {
            m_httpServer->restart();    
        }
        
        if (m_ftpServer->state() != SERVER_WORK)
        {
            m_ftpServer->restart();    
        }
        
        if (m_cliServer->state() != SERVER_WORK)
        {
            m_cliServer->restart();    
        }        
        
        gsys::System::sleep(5);    
    }
    
    G_LOG_OUT();
    
    return G_YES;
}

GResult DfsServer::initService()
{
    G_LOG_IN();
    
    m_httpServer = DfsServerFactory::createServer(HTTP_SERVER);
    m_ftpServer = DfsServerFactory::createServer(FTP_SERVER);
    m_cliServer = DfsServerFactory::createServer(CLI_SERVER);
    
    G_LOG_OUT();
    
    return G_YES;
}
}
