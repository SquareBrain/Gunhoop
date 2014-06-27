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
* @date     2013-11-15
* @note 
*
*  2. 2014-06-21 duye move to gohoop project 
*  1. 2013-11-15 duye Created this file
* 
*/

#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

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

/** 
 * interface define
 */
#define interface			struct;

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
 * limits number for max path length
 */
#define G_PATH_MAX			512

/** 
 * limits number for max file name length
 */
#define G_FILE_NAME_MAX	    256

/** 
 * name space define
 */
/*
namespace GBase { }
namespace GCommon { }
namespace GData { }
namespace GDfs { }
namespace GMapreduce { }
namespace GUtils { }
*/

#define G_NS_END	        }
#define G_NS_GBASE_BEG	    namespace GBase {
#define G_NS_GCOMMON_BEG	namespace GCommon {
#define G_NS_GDATA_BEG	    namespace GData {
#define G_NS_GDFS_BEG	    namespace GDfs {
#define G_NS_GMAPREDUCE_BEG	namespace GMapreduce {
#define G_NS_GUTILS_BEG	    namespace GUtils {