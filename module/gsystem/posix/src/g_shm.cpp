/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_GShm.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-02-22
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2014-02-22 duye Created this file
* 
*/
#include <g_shm.h>

// default GShm size is 10M
static const GUint64 G_DEF_GShm_SIZE = 1024 * 1024 * 10;

GShm::GShm() : m_GShmSize(0), m_GShmAddr(NULL), m_initFlags(false)
{
    m_GShmPath[0] = 0;
}

GShm::GShm(const GInt8* GShmPath, const GUint64 GShmSize) 
    : m_GShmSize(GShmSize), m_GShmAddr(NULL), m_initFlags(false)
{
    setGShmPath(GShmPath);
    init();
}

GShm::~GShm()
{
    uninit();    
}

void GShm::setGShmPath(const GInt8* GShmPath)
{
    GUint32 len = strlen(GShmPath);
    memcpy(m_GShmPath, GShmPath, len);
    m_GShmPath[len] = 0;  
    init();
}

GInt8* GShm::getGShmPath()
{
    return m_GShmPath;
}

void GShm::setGShmSize(const GUint64 GShmSize)
{
    m_GShmSize = GShmSize;    
}

GUint64 GShm::getGShmSize() const
{
    return m_GShmSize;
}
     
GResult GShm::syncGShm()
{
    if (!m_initFlags)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "GShm hasn't init");
        return G_NO;
    }
    
    if (msync(m_GShmAddr, m_GShmSize, MS_SYNC) < 0)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "call msync() failed");
        return SYNC_GShm_FAILED;
    }

    return G_YES;    
} 
 
GResult GShm::writeGShm(const GUint32 offset, const GInt8* data, const GUint32 length)
{
    if (!m_initFlags)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "GShm hasn't init");
        return G_NO;
    } 

    if (data == NULL)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "input data is NULL");
        return WRITE_GShm_PARA_FAILED;
    }
    
    if (length == 0)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "input data length is zero");
        return WRITE_GShm_PARA_FAILED;
    }
    
    if (offset + length > m_GShmSize)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "will write data length over GShm size");
        return WRITE_GShm_PARA_FAILED;
    }

    memcpy((GInt8*)m_GShmAddr + offset, data, length);
    
    return G_YES;        
}

GResult GShm::readGShm(const GUint32 offset, GInt8* buffer, const GUint32 size)
{
    if (!m_initFlags)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "GShm hasn't init");
        return G_NO;
    } 

    if (buffer == NULL)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "output buffer is NULL");
        return WRITE_GShm_PARA_FAILED;
    }
    
    if (size == 0)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "ouput buffer size is zero");
        return WRITE_GShm_PARA_FAILED;
    }
    
    if (offset + size > m_GShmSize)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "will read length over GShm size");
        return WRITE_GShm_PARA_FAILED;
    }

    memcpy((GInt8*)buffer, (GInt8*)m_GShmAddr + offset, size);
    
    return G_YES;     
}

GResult GShm::init()
{
    if (m_initFlags)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "GShm had init");
        return G_NO;
    }

    if (strlen(m_GShmPath) == 0)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "GShm path is empty");
        return GShm_PATH_EMPTY;
    }
    
    GInt32 fd = open(m_GShmPath, O_RDWR | O_CREAT);
    if (fd < 0)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "open GShm file failed");
        return OPEN_GShm_FAILED;
    }

    // setting file size
    if (m_GShmSize == 0)
    {
        m_GShmSize = G_DEF_GShm_SIZE;   
    }
    
    ftruncate(fd, m_GShmSize);
    m_GShmAddr = mmap(NULL, m_GShmSize, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (m_GShmAddr == MAP_FAILED)
    {
        close(fd);
        //G_LOG_ERROR(G_LOG_PREFIX, "call mmap() failed");
        return MMAP_GShm_FAILED;
    }

    close(fd);

    m_initFlags = true;
    
    return G_YES;
}
    
GResult GShm::uninit()
{
    if (!m_initFlags)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "GShm hasn't init");
        return G_NO;
    }
    
    if (munmap(m_GShmAddr, m_GShmSize) < 0)
    {
        //G_LOG_ERROR(G_LOG_PREFIX, "call munmap failed");
        return MUNMAP_GShm_FAILED;
    }

    m_GShmAddr = NULL;

    m_initFlags = false;

    return G_YES;
}
