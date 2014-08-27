#include <g_logger.h>

int main()
{
    G_LOG_INIT();

    G_LOG_ERROR("gutils.logger.example", "error print test");
    G_LOG_WARN("gutils.logger.example", "error print test");
    G_LOG_INFO("gutils.logger.example", "error print test");
    G_LOG_DEBUG("gutils.logger.example", "error print test");
    G_LOG_TRACE("gutils.logger.example", "error print test");
    
    G_LOG_ERROR("gutils.logger.example1", "error print test");
    G_LOG_WARN("gutils.logger.example1", "error print test");
    G_LOG_INFO("gutils.logger.example1", "error print test");
    G_LOG_DEBUG("gutils.logger.example1", "error print test");
    G_LOG_TRACE("gutils.logger.example1", "error print test");    
    
    return 0;
}
