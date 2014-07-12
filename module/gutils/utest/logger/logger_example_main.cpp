#include <g_logger.h>

int main()
{
    G_LOG_INIT();

    G_LOG_ERROR("gutils.logger.example", "error print test");
    
    G_LOG_UNINIT();
    
    return 0;
}
