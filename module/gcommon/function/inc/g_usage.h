/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_usage.h
* @version     
* @brief      
* @author	duye
* @date		2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/
#pragma once

#include <list>
#include <string>

#include <g_type.h>
#include <g_system.h>

namespace gcom {

/**
 * @brief argument type
 */
typedef enum
{
    need,
    option
} ArgType;

class Argument;

/**
 * @brief usage class
 */
class Usage
{
public:
    typedef std::list<Argument> ArgumentList;
    typedef std::list<std::string> ExampleList;

public:  
    Usage();
    ~Usage();
    
    void format(const GInt8* format);
    
    /**
     * @brief add usage example
     * @param [in] example : example
     */
    void addeg(const GInt8* example);
    
    /**
     * @brief add arguments
     * @param [in] shor_cmd : shor command
     * @param [in] arg_type : need or option
     * @param [in] long_cmd : long command
     * @param [in] description : argument description
     */
    void addArg(const GInt8 short_cmd, 
        const ArgType& arg_type, 
        const GInt8* long_cmd, 
        const GInt8* description);
    
    /**
     * @brief print usage
     */
    void print();
    
private:
    ArgumentList        m_argList;
    ExampleList         m_egList;
};

/**
 * @brief argument class
 */
class Argument
{
public:
    Argument();
    ~Argument();
    
private:
    GInt8       m_shortCmd;
    ArgType     m_argType;
    std::string m_longCmd;
    std::string m_description;
};

}
