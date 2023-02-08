#ifdef PLATFORM_WIN
    #include "win_app.hpp"
#elif PLATFORM_LIN
    #include "lin_app.hpp"
#elif PLATFORM_OSX
    #include "osx_app.hpp"
#endif

#include "debug_logger.hpp"

namespace debug_logger
{
    #ifdef DEBUG_VAR

        #if DEBUG_VAR == 0
            Log_Class debuglogger(0);
        #elif DEBUG_VAR == 1
            Log_Class debuglogger(1, debuglog);
        #else
            Log_Class debuglogger(2, debuglog);
        #endif
    #else
        Log_Class debuglogger;
    #endif
}

// NOTE:
// this is recommended by Joe because I will get a multiple-definition problem if I dont do this

// create class object
// class object constructor should create these two ^
// 