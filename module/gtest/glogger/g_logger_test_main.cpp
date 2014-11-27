/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_logger_test_main.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-11-20
* @note 
*

*  1. 2014-11-20 duye Created this file
* 
*/
#include <g_system.h>
#include <g_logger.h>

class LoggerTester
{
public:
    LoggerTester();
    ~LoggerTester();
    
    GResult init();
    GResult uninit();
    GResult printToStdout();
    GResult printToFile();
};

LoggerTester::LoggerTester() {}
LoggerTester::~LoggerTester() {}

GResult LoggerTester::init()
{
    G_LOG_INIT();
    return G_YES;
}

GResult LoggerTester::uninit()
{
    G_LOG_UNINIT();
    return G_YES;
}

GResult LoggerTester::printToStdout()
{
    G_LOG_ERROR("module.a", "module.a error log test");
    G_LOG_WARN("module.b", "module.b warn log test");
    G_LOG_INFO("module.c", "module.c info log test");
    G_LOG_DEBUG("module.d", "module.d debug log test");
    G_LOG_TRACE("module.e", "module.e trace log test");
    
    return G_NO;
}

GResult LoggerTester::printToFile()
{
    G_LOG_ERROR("module.a", "module.a error log test");
    G_LOG_WARN("module.b", "module.b warn log test");
    G_LOG_INFO("module.c", "module.c info log test");
    G_LOG_DEBUG("module.d", "module.d debug log test");
    G_LOG_TRACE("module.e", "module.e trace log test");
    
    return G_NO;
}

int main()
{
    LoggerTester tester;
        
	TestShow("LoggerTester::init()", tester.init());
	TestShow("LoggerTester::printToStdout()", tester.printToStdout());
    TestShow("LoggerTester::printToFile()", tester.printToFile());
	TestShow("LoggerTester::uninit()", tester.uninit());
    
    return 0;
}
