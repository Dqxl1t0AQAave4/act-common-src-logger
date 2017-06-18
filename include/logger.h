#pragma once

#include <afxwin.h>

#include <functional>

namespace logger
{

    using logger_t = std::function < void(CString) > ;

    void log(CString s, ...);

    void log_last_error(DWORD error_message_code);

    void set(logger_t logger);
}