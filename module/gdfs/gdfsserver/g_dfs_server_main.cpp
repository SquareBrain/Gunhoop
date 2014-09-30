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
static gsys::Condition exit_condition;
static const GUint32 exit_timeout = 10; // second

/**
 * @brief dfs server process monitor class
 */
class DfsServerProcessMonitorObserver : public gsys::ProcessMoniterInterface
{
public:
    DfsServerProcessMonitorObserver() {}
    ~DfsServerProcessMonitorObserver() {}
    
    /**
     * @brief handle system signal, when segmentation fault
     * @param [in] sig : signal 
     * @note inherit from base class ProcessMoniterInterface
     */
    void onSegmentationFault(const GInt32 sig)
    {
        G_LOG_ERROR(LOG_PREFIX, "DFS server segmentation fault happened");
        G_LOG_INFO(LOG_PREFIX, "DFS server will stopped, waitting for exit");
        DfsServer::getInstance()->setState(SERVER_FAILED);
        if (!exit_condition.wait(exit_timeout))
        {
            G_LOG_ERROR(LOG_PREFIX, "DFS server can't normal exit, will force exit");
            G_LOG_WARN(LOG_PREFIX, "==============DFS server force exit===========");
        }
    }
    
    /**
     * @brief handle system signal, when ctrl+c
     * @param [in] sig : signal 
     * @note inherit from base class ProcessMoniterInterface
     */
    void onCtrlC(const GInt32 sig)
    {
        G_LOG_INFO(LOG_PREFIX, "DFS server stop by ctrl + c");
        G_LOG_INFO(LOG_PREFIX, "DFS server will stopped, waitting for exit");
        DfsServer::getInstance()->setState(SERVER_STOP);
        if (!exit_condition.wait(exit_timeout))
        {
            G_LOG_ERROR(LOG_PREFIX, "DFS server can't normal exit, will force exit");
            G_LOG_WARN(LOG_PREFIX, "==============DFS server force exit==============");
        }
    }
}

int main(int argc, char** argv)
{
    G_LOG_INIT();
    
    G_LOG_INFO(LOG_PREFIX, "==============DFS server startup===========");
    
    // process monitor
    DfsServerProcessMonitorObserver dfs_server_process_monitor_observer;
    gsys::ProcessMonitor process_monitor(&dfs_server_process_monitor_observer);
    
    if (IS_NO(DFSServer::getInstance()->start()))
    {
        G_LOG_ERROR(LOG_PREFIX, "DFS server startup fault");
        return -1;
    }
    
    while (DfsServer::getInstance()->getState() == SERVER_RUNNING)
    {
        gsys::gsystem::sleep(2);
    }
    
    DfsServer::getInstance()->stop();
    exit_condition.broadcast();
    
    G_LOG_INFO(LOG_PREFIX, "==============DFS server stopped===========");
    
    G_LOG_UNINIT();
    
    return 0;
}
