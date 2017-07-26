#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <sstream>
#include <cstdarg>

#include <act-common/logger_fmt.h>

namespace logger
{

    namespace spi
    {

        struct mutex_holder
        {
            std::mutex _mx;
        };


        template < typename MsgType >
        class logger_t : public mutex_holder
        {

        public:
            
            using message_t = MsgType;
            using logger_fn = std::function < void(const message_t &fn) > ;

        private:

            logger_fn _logger;

        public:

            logger_t() : _logger([](const message_t &fn){})
            {
            }

            void log(const message_t & msg)
            {
                std::lock_guard<std::mutex> guard(_mx);
                _logger(msg);
            }
            
            void set_logger(const logger_fn &fn)
            {
                std::lock_guard<std::mutex> guard(_mx);
                _logger = fn;
            }

            void set_logger(logger_fn &&fn)
            {
                std::lock_guard<std::mutex> guard(_mx);
                _logger = std::move(fn);
            }
        };
    }

    template < typename MsgType, typename Spi = spi::logger_t < MsgType > >
    class logger_t
    {

    public:

        using message_t  = MsgType;
        using spi_t      = Spi;
        using instance_t = logger_t < message_t, spi_t > ;

    private:

        spi_t _log;

    public:
        
        logger_t() = default;

    private:

        logger_t(const instance_t &) = delete;
        logger_t(instance_t &&) = delete;

        instance_t & operator = (const instance_t &) = delete;
        instance_t & operator = (instance_t &&) = delete;

    private:

        static instance_t _instance;

    public:

        static instance_t & get()
        {
            return _instance;
        }

        static instance_t & log(const message_t & msg)
        {
            _instance._log.log(msg);
            return _instance;
        }

        static instance_t & log(message_t && msg)
        {
            _instance._log.log(msg);
            return _instance;
        }

    public:

        spi_t & spi()
        {
            return _log;
        }

    };

    using clog = logger_t < std::string > ;
    using wlog = logger_t < std::wstring > ;
    
    template < typename Log >
    inline typename Log::instance_t & log(const typename Log::message_t & msg)
    {
        return Log::log(msg);
    }

    template < typename Log >
    inline typename Log::instance_t & log(typename Log::message_t && msg)
    {
        return Log::log(std::forward<Log::message_t>(msg));
    }

    template < typename Log, typename MsgType, typename ...Args >
    inline typename Log::instance_t & logf(const MsgType & msg, Args && ...args)
    {
        return Log::log(format<Log::message_t>(msg, std::forward<Args>(args)...));
    }

    template < typename Log, typename MsgType, typename ...Args >
    inline typename Log::instance_t & logf(MsgType && msg, Args && ...args)
    {
        return Log::log(format<Log::message_t>(std::forward<MsgType>(msg), std::forward<Args>(args)...));
    }

    template < typename Log >
    inline typename Log::instance_t & logs(std::function<void(std::basic_ostream<typename Log::message_t::value_type> &)> && fn)
    {
        std::basic_ostringstream<typename Log::message_t::value_type> s;
        fn(s);
        return Log::log(s.str());
    }

    template < typename Log >
    inline typename Log::instance_t & logs(const typename Log::message_t::value_type * msg, ...)
    {
        std::basic_string<typename Log::message_t::value_type> fmt;

        va_list args;

        va_start(args, msg);
        fmt = format_v(msg, args);
        va_end(args);

        return Log::log(fmt);
    }
}