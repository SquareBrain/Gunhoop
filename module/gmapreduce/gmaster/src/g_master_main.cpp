/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_master_main.cpp
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
#include <g_type.h>
#include <g_master.h>

int main()
{
	printf("Gohoop Master Start.\n");
	GMapreduce::Master master;
	master.Start();
	return 0;
}
