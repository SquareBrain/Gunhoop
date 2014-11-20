/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_network_test_main.h
* @version     
* @brief      
* @author	duye
* @date		2014-11-20
* @note 
*
*  1. 2014-11-20 duye Created this file
* 
*/
#include <g_http_test.h>

HttpTester::HttpTester() {}
HttpTester::~HttpTester() {}

GResult HttpTester::start()
{
    gcom::HttpServer httpServer;
    
    
    if (IS_NO(httpServer.start()))
    {
        return G_NO;
    }
    
    return G_YES;
}