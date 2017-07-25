#pragma once

namespace logger
{

    template < typename FmtType, typename MsgType, typename ...Args >
    FmtType format(const MsgType & msg, Args && ...args);
    
    template < typename FmtType, typename MsgType, typename ...Args >
    FmtType format(MsgType && msg, Args && ...args);
}