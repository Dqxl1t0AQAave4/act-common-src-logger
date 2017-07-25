#pragma once

#include <functional>
#include <mutex>
#include <string>

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

        template < typename MsgType2, typename ...Args >
        static instance_t & logf(const MsgType2 & msg, Args && ...args)
        {
            return log(format<message_t>(msg, std::forward<Args>(args)...));
        }

        template < typename MsgType2, typename ...Args >
        static instance_t & logf(MsgType2 && msg, Args && ...args)
        {
            return log(format<message_t>(std::forward<MsgType2>(msg), std::forward<Args>(args)...));
        }

    public:

        spi_t & spi()
        {
            return _log;
        }

    };

    using log  = logger_t < std::string > ;
    using wlog = logger_t < std::wstring > ;
}