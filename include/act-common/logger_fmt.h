#pragma once

#include <cstdarg>
#include <memory>
#include <string>

namespace logger
{

    template < typename FmtType, typename MsgType, typename ...Args >
    FmtType format(const MsgType & msg, Args && ...args);
    
    template < typename FmtType, typename MsgType, typename ...Args >
    FmtType format(MsgType && msg, Args && ...args);

    inline std::wstring format_v(const wchar_t * str, va_list args)
    {
        va_list args0;

        va_copy(args0, args);
        int size = _vscwprintf(str, args0);
        va_end(args0);

        std::unique_ptr<wchar_t[]> buf(new wchar_t[ size + 1 ]); 
        
        va_copy(args0, args);
        #pragma warning(push)
        #pragma warning(disable : 4995)
        #pragma warning(disable : 4996)
		    _vswprintf(buf.get(), str, args0);
        #pragma warning(pop)
        va_end(args0);

        return std::wstring(buf.get(), buf.get() + size);
    }

    inline std::string format_v(const char * str, va_list args)
    {
        va_list args0;

        va_copy(args0, args);
        int size = _vscprintf(str, args0);
        va_end(args0);

        std::unique_ptr<char[]> buf(new char[ size + 1 ]); 
        
        va_copy(args0, args);
        #pragma warning(push)
        #pragma warning(disable : 4995)
        #pragma warning(disable : 4996)
		    vsprintf(buf.get(), str, args0);
        #pragma warning(pop)
        va_end(args0);

        return std::string(buf.get(), buf.get() + size);
    }

    inline std::string format(const char * str, ...)
    {
        std::string fmt;

        va_list args;

        va_start(args, str);
        fmt = format_v(str, args);
        va_end(args);

        return fmt;
    }

    inline std::wstring format(const wchar_t * str, ...)
    {
        std::wstring fmt;

        va_list args;

        va_start(args, str);
        fmt = format_v(str, args);
        va_end(args);

        return fmt;
    }
}