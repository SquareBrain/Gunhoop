/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_type.h
* @version     
* @brief      
* @author   duye
* @date     2014-07-12
* @note 
*
*  1. 2014-07-12 duye Created this file
* 
*/

#pragma once

/** 
 * base type define
 */
typedef char                GInt8;
typedef short		        GInt16;
typedef int                 GInt32;
typedef long long			GInt64;
typedef unsigned char       GUint8;
typedef unsigned short		GUint16;
typedef unsigned int        GUint32;
typedef unsigned long long  GUint64;
typedef int                 GResult;

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <g_type.h>

/** 
 * assert
 */
#define G_ASSERT(x)			assert(x)

/** 
 * result variable for successed
 */
#define G_YES				0

/** 
 * result variable for failed
 */
#define G_NO				-1

/** 
 * limits number for path max length
 */
#define G_PATH_MAX			512

/** 
 * limits number for file name max length
 */
#define G_FILE_NAME_MAX	    256

/** 
 * limits number for error info buffer size
 */
#define G_ERROR_BUF_SIZE	128