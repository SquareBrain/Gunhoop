/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		duye_pipe.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-12-1
* @note 
*
*  1. 2013-12-1 duye Created this file
* 
*/

#include <duye/posix/ipc/inc/duye_pipe.h>

static const D_Int8* LOG_PREFIX = "posix.ipc.pipe";

DUYE_POSIX_NS_BEG

D_Bool Pipe::PosixOpen(const D_Int8* pipeName, const D_Int32 mode)
{
    if (pipeName == NULL)
    {
        return false;
    }

    m_pipefd = open(pipeName, mode);
    if (m_pipefd == -1)
    {
        return false;
    }

    return true;   
}

D_Bool WritePipe::Open(const D_Int8* pipeName)
{
    if (pipeName == NULL)
    {
        return false;
    }

    if (access(pipeName, F_OK) == -1)
    {
        if (mkfifo(pipeName, 0777) != 0)
        {
            POSIX_ERROR(LOG_PREFIX, "mkfifo failed. \n");
            return false;
        }
    }

    return PosixOpen(pipeName, O_WRONLY | O_NONBLOCK);          
}

D_Int32 WritePipe::Write(const D_Int8* data, const D_UInt32 dataLen)
{
    if (m_pipefd == -1)
    {
        return -1;
    }
    
    if (data == NULL)
    {
        return -1;
    }
    
    if (write(m_pipefd, data, dataLen) == -1)
    {
        return -1;
    }
    
    return dataLen;    
}

D_Bool ReadPipe::Open(const D_Int8* pipeName)
{
    if (access(pipeName, F_OK) == -1)
    {
        if (mkfifo(pipeName, 0777) != 0)
        {
            POSIX_ERROR(LOG_PREFIX, "mkfifo failed. \n");
            return false;
        }
    }

    return PosixOpen(pipeName, O_RDONLY);        
}

D_Int32 ReadPipe::Read(D_Int8* buffer, const D_UInt32 bufferSize)
{
    if (m_pipefd == -1)
    {
        return -1;
    }

    if (buffer == NULL)
    {
        return -1;
    }
    
    D_Int32 bytes = read(m_pipefd, buffer, bufferSize);
    if (bytes == -1)
    {
        return -1;
    }

    buffer[bytes] = '\0';
    
    return bytes;    
}
 
DUYE_POSIX_NS_END
