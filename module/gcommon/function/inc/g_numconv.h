/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_numconv.h
* @version     
* @brief      
* @author   duye
* @date     2014-07-02
* @note 
*
*  1. 2014-07-02 duye Created this file
* 
*/

#pragma once

#include <g_system.h>
#include <g_namespace.h>

G_NS_GCOMMON_BEG

/** 
 * @brief number convert wrapper
 */
class NumConv
{
public:
    /**
     * @brief convert string to int16
	 * @param [in]str : input number with string format
	 * @param [out]num : output number
	 * @return G_YES/G_NO
     * @note 
     */			
    static GResult strToInt16(GInt8* str, GInt16& num);  

    /**
     * @brief convert string to int32
	 * @param [in]str : input number with string format
	 * @param [out]num : output number
	 * @return G_YES/G_NO
     * @note 
     */			
    static GResult strToInt32(GInt8* str, GInt32& num); 
 
    /**
     * @brief convert string to int64
	 * @param [in]str : input number with string format
	 * @param [out]num : output number
	 * @return G_YES/G_NO
     * @note 
     */			
    static GResult strToInt64(GInt8* str, GInt64& num); 
  
    /**
     * @brief convert string to uint16
	 * @param [in]str : input number with string format
	 * @param [out]num : output number
	 * @return G_YES/G_NO
     * @note 
     */			
    static GResult strToUint16(GInt8* str, GUint16& num);  

    /**
     * @brief convert string to uint32
	 * @param [in]str : input number with string format
	 * @param [out]num : output number
	 * @return G_YES/G_NO
     * @note 
     */			
    static GResult strToUint32(GInt8* str, GUint32& num);     

    /**
     * @brief convert string to uint64
	 * @param [in]str : input number with string format
	 * @param [out]num : output number
	 * @return G_YES/G_NO
     * @note 
     */			
    static GResult strToUint64(GInt8* str, GUint64& num); 

    /**
     * @brief convert number to number
	 * @param [in]num : input number with string format
	 * @param [out]buffer : output buffer
	 * @param [in]size : output buffer size
	 * @return G_YES/G_NO
     * @note 
     */			
    static GResult numToStr(GInt64& num, GInt8* buffer, const GUint32 size); 

    /**
     * @brief convert number to unsigned number
	 * @param [in]num : input number with string format
	 * @param [out]buffer : output buffer
	 * @param [in]size : output buffer size
	 * @return G_YES/G_NO
     * @note 
     */			
    static GResult numToStr(GUint64& num, GInt8* buffer, const GUint32 size);    
};

G_NS_END
