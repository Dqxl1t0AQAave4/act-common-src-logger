#pragma once

#include <afxwin.h>

#include <functional>

namespace logger
{

    /**
     * The logger function type.
     *
     * Logger function called each time logger method is called
     * with the fully populated string parameter.
     */
    using logger_t = std::function < void(CString) > ;
    
    
    /**
     * The function operates in the same way as CString:Format.
     * 
     * Thread-safe.
     */
    void log(CString s, ...);
    
    
    /**
     * Logs formatted error message represented by the given
     * WinAPI error message code.
     *
     * Thread-safe.
     */
    void log_system_error(DWORD error_message_code);

    
    /**
     * Tells the library use the given logger.
     *
     * Thread-safe.
     * 
     * The initial value of logger function is
     * blank function [](CString){}.
     */
    void set(logger_t logger);
}