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
#include <g_dfs_server.h>

int main()
{
    
    DFSServer dfs_server;
    
    if (IS_NO(dfs_server.start()))
    {
        
    }
    
    for (;;)
    {
        gsys::gsystem::sleep(2);
    }
    
    return 0;
}
