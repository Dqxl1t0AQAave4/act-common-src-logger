#pragma once

#include <act-common/logger.h>
#include <Windows.h>

namespace logger
{

    struct sys_error
    {
        DWORD error;
    };

    template <>
    inline clog::message_t format(const sys_error & msg)
    {
        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, msg.error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &messageBuffer, 0, NULL);

        /* FormatMessage adds '\r\n' that is redundant for us */
        clog::message_t str(messageBuffer);
        str.erase(str.length() - 2, 2);

        LocalFree(messageBuffer);

        return str;
    }

    template <>
    inline clog::message_t format(sys_error && msg)
    {
        clog::message_t fmt = format<clog::message_t>((const sys_error & )msg);
        msg.error = 0;
        return fmt;
    }

    template <>
    inline wlog::message_t format(const sys_error & msg)
    {
        LPWSTR messageBuffer = nullptr;
        size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, msg.error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &messageBuffer, 0, NULL);

        /* FormatMessage adds '\r\n' that is redundant for us */
        wlog::message_t str(messageBuffer);
        str.erase(str.length() - 2, 2);

        LocalFree(messageBuffer);

        return str;
    }

    template <>
    inline wlog::message_t format(sys_error && msg)
    {
        wlog::message_t fmt = format<wlog::message_t>((const sys_error & )msg);
        msg.error = 0;
        return fmt;
    }
}