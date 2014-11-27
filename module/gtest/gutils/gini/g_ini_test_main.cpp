/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_ini_test_main.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-11-27
* @note 
*
*  1. 2014-11-27 duye Created this file
* 
*/
#include <g_system.h>
#include <g_ini.h>

using namespace std;

class IniTester
{
public:
    IniTester();
    ~IniTester();
    
    GResult read();
    GResult write();
};

IniTester::IniTester() {}
IniTester::~IniTester() {}

GResult IniTester::read()
{
    return G_YES;
}

GResult IniTester::write()
{
    return G_YES;
}

void TestShow(const GInt8* function, const GResult result)
{
    printf("%s   %s \n", function, result == G_YES ? "YES" : "NO");
}

int main(int argc, char** argv) 
{
	IniTester tester;
    
    TestShow("InitTester::read()", tester.read());
    TestShow("InitTester::write()", tester.write());
    
	return 0;
}
