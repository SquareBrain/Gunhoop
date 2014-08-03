/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_slave_main.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-06-22
* @note 
*
*  1. 2014-06-22 duye Created this file
* 
*/
#include <stdio.h>
#include <g_slave.h>

int main()
{
	printf("Gohoop Master Start.\n");

	GMapreduce::Slave slave;
	slave.start();

	return 0;
}
