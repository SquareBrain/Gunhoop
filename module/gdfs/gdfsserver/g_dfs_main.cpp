/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_dfs_man.h
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
class DFSProcessMonitor : public ProcessMoniterInterface
{
public:
    DFSProcessMonitor() {}
    ~DFSProcessMonitor() {}
    
    /**
     * @brief handle system signal, when segmentation fault
     * @param [in] sig : signal 
     * @note inherit from base class ProcessMoniterInterface
     */
    void onSegmentation(const GInt32 sig)
    {
        DFSServer::getInstance()->setState(SERVER_FAILED);
    }
    
    /**
     * @brief handle system signal, when ctrl+c
     * @param [in] sig : signal 
     * @note inherit from base class ProcessMoniterInterface
     */
    void onCtrlC(const GInt32 sig)
    {
        DFSServer::getInstance()->setState(SERVER_STOP);
    }
}

int main()
{
    G_LOG_INIT();
    
    // process monitor
    DFSProcessMonitor dfs_process_monitor;
    gsys::ProcessMonitor process_monitor(&dfs_process_monitor);
    
    if (IS_NO(DFSServer::getInstance()->start()))
    {
        G_LOG_INFO(LOG_PREFIX, "DFS Server Startup Fault");
        return -1;
    }
    
    while (DFSServer::getInstance()->getState() == SERVER_RUNNING
    {
        gsys::gsystem::sleep(2);
    }
    
    DFSServer::getInstance()->stop();
    
    
    
    G_LOG_UNINIT();
    
    return 0;
}
