/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_result.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
* @note 
*
*  2. 2014-06-21 duye move to gohoop project 
*  1. 2013-11-15 duye Created this file
* 
*/

#pragma once

/** 
 * @brief result variable for successed
 */
#define G_YES   0

/** 
 * @brief result variable for failed
 */
#define G_NO    -1

/** 
 * base error code
 */
#define G_ERROR_GENERAL_BASE                -1000
#define G_ERROR_SYSTEM_BASE                 -2000
#define G_ERROR_UTILS_BASE                  -3000
#define G_ERROR_COMMON_BASE                 -4000

/** 
 * general error code
 */
#define G_ERROR_INVALID_PARAMETERS          (G_ERROR_GENERAL_BASE - 1)

/** 
 * gsystem error code
 */
#define G_ERROR_NO_FILE                     (G_ERROR_SYSTEM_BASE - 1)