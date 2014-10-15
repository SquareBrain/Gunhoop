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
* @date	    2014-07-12
* @note 
*
*  1. 2014-07-12 duye Created this file
* 
*/
#pragma once

#include <assert.h>

/** 
 * @brief base type define
 */
typedef char                GInt8;
typedef short		    GInt16;
typedef int		    GInt32;
typedef long long	    GInt64;
typedef unsigned char	    GUint8;
typedef unsigned short	    GUint16;
typedef unsigned int	    GUint32;
typedef unsigned long long  GUint64;
typedef int		    GResult;

/** 
 * @brief assert
 */
#define G_ASSERT(x) assert(x)

/** 
 * @brief limits number for path max length
 */
#define G_PATH_MAX 512

/** 
 * @brief limits number for file name max length
 */
#define G_FILE_NAME_MAX 256

/** 
 * @brief limits number for error info buffer size
 */
#define G_ERROR_BUF_SIZE 128
