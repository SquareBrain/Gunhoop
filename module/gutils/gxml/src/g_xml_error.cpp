/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_xml_error.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-8-7
* @note 
*
*  1. 2014-8-7 duye Created this file
* 
*/
#include <g_xml.h>

using namespace gutils;

// The goal of the seperate error file is to make the first
// step towards localization. tinyxml (currently) only supports
// english error messages, but the could now be translated.
//
// It also cleans up the code a bit.
//

namespace gutils {

const char* XmlBase::m_errorString[XmlBase::GXML_ERROR_STRING_COUNT] = 
{
	"No error",
	"Error",
	"Failed to open file",
	"Error parsing Element.",
	"Failed to read Element name",
	"Error reading Element value.",
	"Error reading Attributes.",
	"Error: empty tag.",
	"Error reading end tag.",
	"Error parsing Unknown.",
	"Error parsing Comment.",
	"Error parsing Declaration.",
	"Error document empty.",
	"Error null (0) or unexpected EOF found in input stream.",
	"Error parsing CDATA.",
	"Error when XmlDocument added to document, because XmlDocument can only be at the root.",
};
}
