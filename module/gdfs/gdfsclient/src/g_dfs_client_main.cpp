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

class DfsClientProcessMonitorObserver;

int main(int argc, char** argv)
{
    G_LOG_INIT();
    
    G_LOG_INFO(LOG_PREFIX, "==============DFS client startup===========");
    
    gsys::Usage usage;
    usage.format("dfsclient [-s] [-p] [OPTION...]");
    usage.addExample("dfsclient -s 192.168.5.122 -p 8080");
    usage.addExample("dfsclient -s 192.168.5.122 -p 8080 -i eth0");
    usage.addOpt('h', gsys::Usage::option, "help", "print help information");
    usage.addOpt('s', gsys::Usage::need, "server_ip", "the DFS server ip, eg : 192.168.5.122");
    usage.addOpt('p', gsys::Usage::need, "server_port", "the DFS server port, eg : 8080");
    usage.addOpt('i', gsys::Usage::option, "local_interface", "the local communication interface, default is eth0");
    
    // process monitor
    DfsClientProcessMonitorObserver dfs_client_process_monitor_observer;
    gsys::ProcessMonitor process_monitor(&dfs_client_process_monitor_observer);

    // get help
    std::string help;
    if (IS_YES(gsys::System::getOptArg(argc, argv, "h", help)))
    {
        usage.print();
        return 0;
    }    
    
    // get server ip
    std::string server_ip;
    if (IS_NO(gsys::System::getOptArg(argc, argv, "s", server_ip)))
    {
        G_LOG_ERROR(LOG_PREFIX, "Parser argument -h for DFS server IP failed");        
        return -1;
    }
    
    // get server port
    GUint16 server_port = 0;
    if (IS_NO(gsys::System::getOptArg(argc, argv, "p", server_port)))
    {
        G_LOG_ERROR(LOG_PREFIX, "Parser argument -p for DFS server port failed");        
        return -1;
    }
    
    // get communication interface
    std::string interface;
    if (IS_NO(gsys::System::getOptArg(argc, argv, "i", interface)))
    {
        G_LOG_WARN(LOG_PREFIX, "Parser argument -p for DFS server port failed");
    }    
    
    // init dfs client
    gdfs::DfsClient dfs_client;
    if (IS_NO(dfs_client->start(server_ip, server_port)))
    {
        G_LOG_ERROR(LOG_PREFIX, "Try to connect DFS server failed");
        return -1;
    }
    
    exit_condition.wait();
    
    G_LOG_INFO(LOG_PREFIX, "==============DFS client exit===========");
    
    G_LOG_UNINIT();
    
    return 0;
}

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
};
