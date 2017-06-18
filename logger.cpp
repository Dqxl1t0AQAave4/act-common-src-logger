#include "stdafx.h"

#include <logger.h>

#include <mutex>

namespace logger
{
    namespace
    {
        static logger_t _log([] (CString) {});
        static std::mutex log_guard;
    }

    void log(CString s, ...)
    {
        std::lock_guard<std::mutex> guard(log_guard);
        va_list args;
        va_start(args, s);
        CString f; f.FormatV(s, args);
        _log(f);
        va_end(args);
    }

    void log_last_error(DWORD error_message_code)
    {
        LPTSTR messageBuffer = nullptr;
        size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                    NULL, error_message_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &messageBuffer, 0, NULL);
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