#include "stdafx.h"

#include <logger.h>

#include <mutex>

namespace logger
{
    namespace
    {
        logger_t _log([] (CString) {});
        std::mutex log_guard;
    }

    void log(CString s, ...)
    {
        va_list args;
        va_start(args, s);
        CString f; f.FormatV(s, args);
        va_end(args);

        {
            std::lock_guard<std::mutex> guard(log_guard);
            _log(f);
        }
    }

    void log_system_error(DWORD error_message_code)
    {
        LPTSTR messageBuffer = nullptr;
        size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                    NULL, error_message_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &messageBuffer, 0, NULL);

        /* FormatMessage adds '\r\n' that is redundant for us */
        CString str(messageBuffer);
        str.TrimRight(_T("\r\n"));

        log(str);

        LocalFree(messageBuffer);
    }

    void set(logger_t logger)
    {
        std::lock_guard<std::mutex> guard(log_guard);
        _log = logger;
    }
}