/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_dfs_server_main.h
* @version     
* @brief      
* @author   duye
* @date     2014-09-28
* @note 
*
*  1. 2014-09-28 duye Created this file
* 
*/
#include <g_logger.h>
#include <g_system.h>
#include <g_dfs_server.h>

static const GInt8* LOG_PREFIX = "dfs.server.main";

/**
 * @brief dfs server process monitor class
 */
class DfsProcessMonitor : public gcom::ProcessObserver
{
public:
    DfsProcessMonitor() {}
    ~DfsProcessMonitor() {}
    
    /**
     * @brief handle system signal, when segmentation fault
     * @note inherit from base class ProcessMoniterInterface
     */
    void onSegFault()
    {
        G_LOG_ERROR(LOG_PREFIX, "DFS server segmentation fault happened");
        G_LOG_INFO(LOG_PREFIX, "DFS server will stopped, waitting for exit");
        DfsServer::instance().setState(HOST_SERVER_FAULT);
        if (!gcom::ProcessTracker::instance().wait())
        {
            G_LOG_ERROR(LOG_PREFIX, "DFS server can't normal exit, will force exit");
            G_LOG_WARN(LOG_PREFIX, "==============DFS server force exit===========");
        }
    }
    
    /**
     * @brief handle system signal, when ctrl+c
     * @note inherit from base class ProcessMoniterInterface
     */
    void onCtrlC()
    {
        G_LOG_INFO(LOG_PREFIX, "DFS server stop by ctrl + c");
        G_LOG_INFO(LOG_PREFIX, "DFS server will stopped, waitting for exit");
        DfsServer::instance()->setState(HOST_SERVER_STOP);
        if (!gcom::ProcessTracker::instance().wait())
        {
            G_LOG_ERROR(LOG_PREFIX, "DFS server can't normal exit, will force exit");
            G_LOG_WARN(LOG_PREFIX, "==============DFS server force exit==============");
        }
    }
    
    /**
     * @brief handle system signal, when others signal
     * @param [in] sig : signal
     * @note inherit from base class ProcessMoniterInterface
     */    
    void onOther(const GInt32 sig) 
    {
        G_LOG_IN();
        G_LOG_OUT();
    }
}

int main(int argc, char** argv)
{
    G_LOG_INIT();
    
    G_LOG_INFO(LOG_PREFIX, "==============DFS server startup===========");
    
    // process monitor
    DfsProcessMonitor dfs_process_monitor;
    gcom::ProcessTracker::instance().addObserver(&dfs_process_monitor);
    
    if (IS_NO(DfdServer::instance().start()))
    {
        G_LOG_ERROR(LOG_PREFIX, "DFS server startup fault");
        return -1;
    }
    
    while (DfsServer::instance().state() == G_HSERVER_WORK)
    {
        gsys::gsystem::sleep(2);
    }
    
    DfsServer::instance().stop();
    gcom::ProcessTracker::instance().wakeup();
    
    G_LOG_INFO(LOG_PREFIX, "==============DFS server stopped===========");
    
    G_LOG_UNINIT();
    
    return 0;
}
