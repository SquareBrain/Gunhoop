/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_logger_test_main.h
* @version     
* @brief      
* @author	duye
* @date		2014-11-20
* @note 
*

*  1. 2014-11-20 duye Created this file
* 
*/
#include <g_logger.h>

int main()
{
    G_LOG_INIT();

    G_LOG_ERROR("module.a", "module.a error log test");
    G_LOG_WARN("module.b", "module.b warn log test");
    G_LOG_INFO("module.c", "module.c info log test");
    G_LOG_DEBUG("module.d", "module.d debug log test");
    G_LOG_TRACE("module.e", "module.e trace log test");
    
    G_LOG_UNINIT();
    
    return 0;
}
