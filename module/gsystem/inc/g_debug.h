/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	  	g_debug.h
* @version     
* @brief      
* @author 	duye
* @date     	2013-11-15
* @note 
*
*  2. 2014-06-21 duye move to gohoop project 
*  1. 2013-11-15 duye Created this file
* 
*/

#pragma once

#include <g_result.h>

#define G_CHECK_RESULT(_x)        \
do {                              \
    GResult _result = (_x);       \
    if (_result != G_YES) {       \
        return _result;           \
    }                             \
} while(0)

#define G_CHECK_POINTER(_p)                 \
do {                                        \
    if ((_p) == NULL) {                     \
        return G_ERROR_INVALID_PARAMETERS;  \
    }                                       \
} while(0)

#define G_YESED(result) ((result) == G_YES)
#define G_NOED(result)  ((result) == G_NO)