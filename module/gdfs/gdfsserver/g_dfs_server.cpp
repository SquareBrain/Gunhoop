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

namespace gdfs {

DFSServer::DFSServer() : m_serverState(SERVER_INIT) {}

DFSServer::DFSServer(const std::string& dfs_cfg_file_path) 
    : m_serverState(SERVER_INIT)
    , m_dfsCfgFilePath(dfs_cfg_file_path) {}

DFSServer::~DFSServer() {}

GResult DFSServer::start()
{
    if (m_serverState == SERVER_ON)
    {
        return G_YES;
    }
    
    IS_NO_R(loadCfg());
    IS_NO_R(createService());
    
    m_serverState = SERVER_ON;
     
    return G_YES;
}

GResult DFSServer::stop()
{
    return G_NO;
}

void DFSServer::setState(const DFSServerState state)
{
    m_serverState = state;
}

DFSServerState DFSServer::getState()
{
    return m_serverState;
}

DFSServerState DFSServer::run()
{
    return G_YES;
}

DFSServerState DFSServer::loadCfg()
{
    if (m_dfsCfgFilePath.empty())
    {
        m_dfsCfgFilePath = DEF_DFS_CFG_FILE_PATH;
    }
    
    if (!m_dfsCfgFile.loadFile(m_dfsCfgFilePath))
    {
        m_serverState = SERVER_FAILED;
        return G_NO;
    }
    
    return G_YES;
}

}
