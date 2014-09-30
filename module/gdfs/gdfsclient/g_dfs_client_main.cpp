/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_dfs_client_main.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-08-30
* @note 
*
*  1. 2014-08-30 duye Created this file
* 
*/
#include <g_system.h>
#include <g_logger.h>
#include <g_dfs_client.h>

static const GInt8* LOG_PREFIX = "dfs.client.main";
static gsys::Condition exit_condition;

/**
 * @brief dfs server process monitor class
 */
class DfsClientProcessMonitorObserver : public gsys::ProcessMoniterInterface
{
public:
    DfsClientProcessMonitorObserver() {}
    ~DfsClientProcessMonitorObserver() {}
    
    /**
     * @brief handle system signal, when segmentation fault
     * @param [in] sig : signal 
     * @note inherit from base class ProcessMoniterInterface
     */
    void onSegmentationFault(const GInt32 sig)
    {
        G_LOG_ERROR(LOG_PREFIX, "DFS client segmentation fault happened");
        G_LOG_INFO(LOG_PREFIX, "DFS client will stopped, waitting for exit");
        exit_condition.signal();
    }
    
    /**
     * @brief handle system signal, when ctrl+c
     * @param [in] sig : signal 
     * @note inherit from base class ProcessMoniterInterface
     */
    void onCtrlC(const GInt32 sig)
    {
        G_LOG_INFO(LOG_PREFIX, "DFS client stop by ctrl + c");
        G_LOG_INFO(LOG_PREFIX, "DFS client will stopped, waitting for exit");
        exit_condition.signal();
    }
}

int main(int argc, char** argv)
{
    G_LOG_INIT();
    
    G_LOG_INFO(LOG_PREFIX, "==============DFS client startup===========");
    
    // process monitor
    DfsClientProcessMonitorObserver dfs_client_process_monitor_observer;
    gsys::ProcessMonitor process_monitor(&dfs_client_process_monitor_observer);
    
    std::string server_ip;
    GInt8 server_port = 0;
    
    GInt8 cmd = 0;
    while ((cmd = gsys::System::getopt(argc, argv, "h:p")))
    {
        switch (cmd)
        {
            case 'h':
                server_ip.assign(optarg);
                break;
            case 'p':
                server_port = std::
                break;
            default:
                break;
        }
    }
    
    if (server_ip.empty() || server_port == 0)
    {
        G_LOG_ERROR(LOG_PREFIX, "Argument -h or -p error");
        return -1;
    }
    
    // init dfs client
    DfsClient dfs_client;
    if (IS_NO(dfs_client->start()))
    {
        G_LOG_ERROR(LOG_PREFIX, "Try to connect DFS server failed");
        return -1;
    }
    
    exit_condition.wait();
    
    G_LOG_INFO(LOG_PREFIX, "==============DFS client exit===========");
    
    G_LOG_UNINIT();
    
    return 0;
}
