/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_pipe.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-12-1
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2013-12-01 duye Created this file
* 
*/
#include <g_pipe.h>

bool GPipe::orgOpen(const GInt8* pipeName, const GInt32 mode)
{
    if (pipeName == NULL)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "GPipe name is NULL");
        return false;
    }
    
    if (access(pipeName, F_OK) == -1)
    {
        if (mkfifo(pipeName, 0777) != 0)
        {
            //G_LOG_ERROR(G_LOG_PREFIX, "call mkfifo() failed");
            return false;
        }
    }    

    m_pipefd = open(pipeName, mode);
    if (m_pipefd == -1)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "open GPipe '%s' failed", GPipeName);
        return false;
    }

    return true;   
}

bool GWritePipe::openPipe(const GInt8* pipeName)
{
    return orgOpen(pipeName, O_WRONLY | O_NONBLOCK);          
}

GInt64 GWritePipe::writeData(const GInt8* data, const GUint64 length)
{
    if (m_pipefd == -1)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "GPipe hasn't open");
        return G_NO;
    }
    
    if (data == NULL)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "input data is NULL");
        return G_NO;
    }

    if (length == 0)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "input data size is zero");
        return G_NO;        
    }    

    GInt64 bytes = -1;
    if ((bytes = write(m_pipefd, data, length)) == -1)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "write GPipe failed");
        return G_NO;
    }
    
    return bytes;    
}

bool GReadPipe::openPipe(const GInt8* pipeName)
{
    return orgOpen(pipeName, O_RDONLY);        
}

GInt64 GReadPipe::readData(GInt8* buffer, const GUint64 size)
{
    if (m_pipefd == -1)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "GPipe hasn't open");
        return G_NO;
    }

    if (buffer == NULL)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "output buffer is NULL");
        return G_NO;
    }

    if (size == 0)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "output buffer size is zero");
        return G_NO;        
    }
    
    GInt32 bytes = -1;
    if ((bytes = read(m_pipefd, buffer, size)) == -1)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "read GPipe failed");
        return G_NO;
    }
    
    return bytes;    
}
