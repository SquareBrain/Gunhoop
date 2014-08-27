#include <g_logger.h>

int main()
{
    G_LOG_INIT();

    G_LOG_ERROR("module.a", "module.a error log test");
    G_LOG_WARN("module.b", "module.b warn log test");
    G_LOG_INFO("module.c", "module.c info log test");
    G_LOG_DEBUG("module.d", "module.d debug log test");
    G_LOG_TRACE("module.e", "module.e trace log test");
    
    return 0;
}
