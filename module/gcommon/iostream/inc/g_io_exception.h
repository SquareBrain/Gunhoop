/************************************************************************************
**
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file g_io_exception.h
* @version
* @brief
* @author ylh
* @date 2014-07-22
* @note
*
* 1. 2014-07-22 ylh Created this file
*
*/

#pragma once

#include <string>
#include <stdexcept>

inline std::string EXCEPTION_DESCRIPTION(const std::string& desc)
{
	return (std::string("File:") + std::string(__FILE__) + \
	std::string(", Line:") + std::to_string(__LINE__) +\
	std::string(", exception:") + std::string(desc));
}

G_NS_GCOMMON_BEG

class EOFException : public std::exception
{
};

G_NS_END

