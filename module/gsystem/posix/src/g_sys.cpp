/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_GSys.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-03-04
* @note 
*
*  1. 2014-03-04 duye Created this file
* 
*/
#include <g_sys.h>

void GSys::gsleep(const GUint64 time)
{
    sleep(time);
}

void GSys::gusleep(const GUint64 time)
{
    usleep(time);
}
