/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_timeconv.h
* @version     
* @brief      
* @author   duye
* @date	    2014-03-09
* @note 
*
*  2. 2014-06-21 duye move to gohoop project 
*  1. 2014-03-09 duye Created this file
* 
*/
#pragma once

#include <string>
#include <list>
#include <g_system.h>

namespace gcom {

/** 
 * @brief time convert tool class
 */
class Convert
{
public:
    /**
     * @brief split string
     * @param [in] src_str : input string
     * @param [in] token : split char
     * @param [out] split_list : output string list
     */
    static GResult splitString(const std::string& src_str, const GInt8 token, std::list<std::string>& split_list);

    /**
     * @brief split string
     * @param [in] src_str : input string
     * @param [in] token : split string
     * @param [out] split_list : output string list
     */    
    static GResult splitString(const std::string& src_str, const std::string& token, std::list<std::string>& split_list);
    
    /**
     * @brief convert to upper
     * @param [in/out] ch : input charactor
     * @param [in/out] str : input string
     */
    static void toupper(GInt8& ch);
    static void toupper(GInt8* str);
    static void toupper(std::string& str);
    
    /**
     * @brief convert to upper
     * @param [in/out] ch : input charactor
     * @param [in/out] str : input string
     */
    static void tolower(GInt8& ch);
    static void tolower(GInt8* str);
    static void tolower(std::string& str);    
};
}
