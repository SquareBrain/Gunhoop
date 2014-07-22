/************************************************************************************
**
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file g_inputstream.h
* @version
* @brief
* @author ylh
* @date 2014-07-02
* @note
*
* 1. 2014-07-02 ylh Created this file
*
*/

#pragma once

#include <string>

inline const std::string& EXCEPTION_DESCRIPTION(const std::string& desc)
{
	return (std::string("File:") + std::string(__FILE__) + \
	std::string(", Line:") + std::to_string(__LINE__) +\
	std::string(", exception:") + std::string(desc));
}
