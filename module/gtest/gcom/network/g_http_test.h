/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_http_test.h
* @version     
* @brief      
* @author	duye
* @date		2014-11-20
* @note 
*
*  1. 2014-11-20 duye Created this file
* 
*/
#pragam once

#include <g_common.h>

/**
 * @brief http tester
 */
class HttpTester : public Singleton<HttpTester>
{
public:
    HttpTester();
    ~HttpTester();
    
    /**
     * @brief start test
     * @return G_YES/G_NO
     */
    GResult start();
};