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
* @date	    2013-11-15
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

#define IS_YES(_ret) ((_ret) == G_YES)
#define IS_NO(_ret) ((_ret) == G_NO)

/**
 * @brief _x is G_YES, return void
 */
#define IS_YES_R(_x) \
do \
{ \
    GResult _ret = (_x); \
    if (_ret == G_YES) \
    { \
        return; \
    } \
} while(0)

/**
 * @brief _x is G_YES, return G_YES
 */
#define IS_YES_RX(_x) \
do \
{ \
    GResult _ret = (_x); \
    if (_ret == G_YES) \
    { \
        return _ret; \
    } \
} while(0)

/**
 * @brief _x is G_NO, return void
 */
#define IS_NO_R(_x) \
do \
{ \
    GResult _ret = (_x); \
    if (_ret == G_NO) \
    { \
        return; \
    } \
} while(0)

/**
 * @brief _x is G_NO, return G_NO
 */
#define IS_NO_RX(_x) \
do \
{ \
    GResult _ret = (_x); \
    if (_ret == G_NO) \
    { \
        return _ret; \
    } \
} while(0)

#define IS_NULL(_x) ((_x) == nullptr)

/**
 * @brief parameter is NULL, return void
 */
#define IS_NULL_R(_x) \
do \
{ \
    if ((_x) == nullptr) \
    { \
        return 0; \
    } \
} while(0)

/**
 * @brief parameter is NULL, return _ret
 */
#define IS_NULL_RX(_x, _ret) \
do \
{ \
    if ((_x) == nullptr) \
    { \
        return (_ret); \
    } \
} while(0)

/**
 * @brief parameter is NULL, return error code
 */
#define IS_NULL_RE(_p) \
do \
{ \
    if ((_p) == nullptr) \
    { \
    	return G_ERROR_INVALID_PARAMETERS; \
    } \
} while(0)
