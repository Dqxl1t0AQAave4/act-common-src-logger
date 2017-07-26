# `act-common` - `/src/logger-simple-api`

Является частью проекта [`act-common`](https://github.com/Dqxl1t0AQAave4/act-common).

Простейший потокобезопасный логгер.

## Заголовочные файлы

```c++
#include <act-common/logger.h>          // (1)
#include <act-common/logger_fmt.h>      // (2)
#include <act-common/logger_win.h>      // (3)
#include <act-common/logger_impl.h>     // (4)
```

**(1)** Основные определения. Классы `logger_t<>`, `spi::logger_t<>`. Алиасы типов `clog`, `wlog`. Функции `log<>()`, `logf<>()`, `logs<>()`, `logss<>()`.

**(2)** Объявления форматтеров `format<>()`. Определения `printf`-like форматтеров.

**(3)** Дополнительные специализации форматтеров для WinAPI. Класс `sys_error` и его форматтер.

**(4)** Определение статических переменных логгеров `clog` и `wlog`.

### Пространства имен

```c++
namespace logger
{
    namespace spi { }
}
```

## API в примерах

```c++
#include <act-common/logger.h>

/* Установка логгер-функций */
logger::clog::get().spi().set_logger([] (const std::string & m) { std::cout << m << std::endl; });
logger::wlog::get().spi().set_logger([] (const std::wstring & m) { std::wcout << m << std::endl; });

//...

/* Логирование строк */
logger::log<logger::clog>("String"); // prints `String`
logger::clog::log("String"); // same as prev.

/* Логирование форматированных строк */
logger::logs<logger::clog>("Mr.%s is No.%d", "Simpson", 1); // prints `Mr.Simpson is No.1`
logger::log<logger::clog>(logger::format("Mr.%s is No.%d", "Simpson", 1)); // prints `Mr.Simpson is No.1`
logger::logs<logger::clog>([] (std::ostream & s) {
    s << "Mr." << "Simpson" << " is No." << 1; }); // prints `Mr.Simpson is No.1`

/* Логирование объектов */
logger::logss<logger::clog>(123); // prints `123`
logger::logs<logger::clog>([] (std::ostream & s) { s << 123; }); // same as prev.

/* Логирование с применением форматтеров */
#include <act-common/logger_win.h>
logger::logf<logger::clog>(logger::sys_error{0}); // prints e.g. `Операция успешно завершена.`
logger::log<logger::clog>(logger::format<std::string>(logger::sys_error{0})); // same as prev.

/* Специализация и перегрузка форматтеров */

// Объявления форматтеров могут быть включены отдельно от `logger.h`
#include <act-common/logger_fmt.h>

struct s1 { std::string first_name; std::string last_name; };
struct s2 { std::string first_name; std::string last_name; };

namespace logger
{
    
    enum format_type
    {
        format_type_short,
        format_type_long
    };
    
    /* Specialization */
    
    template <> std::string format(const s1 &s, format_type &&t) // lvalue
    {
        if (t == format_type_short) return s.first_name;
        return s.first_name + " " + s.last_name;
    }
    
    template <> std::string format(s1 &&s, format_type &&t) // rvalue
    {
        if (t == format_type_short) return s.first_name;
        return s.first_name + " " + s.last_name;
    }

    /* Overloading */
    
    template <typename T> T format(const s2 &s, format_type t);
    template <typename T> T format(s2 &&s, format_type t);
    
    template <> std::string format(const s2 &s, format_type t) // lvalue
    {
        if (t == format_type_short) return s.first_name;
        return s.first_name + " " + s.last_name;
    }
    
    template <> std::string format(s2 &&s, format_type t) // rvalue
    {
        if (t == format_type_short) return s.first_name;
        return s.first_name + " " + s.last_name;
    }
}

// Объявления форматтеров могут быть включены отдельно от `logger.h`
#include <act-common/logger.h>

// Specialized `format`
logger::logf<logger::clog>(s1{"Homer", "Simpson"}, logger::format_type_short); // prints `Homer`
logger::logf<logger::clog>(s1{"Homer", "Simpson"}, logger::format_type_long); // prints `Homer Simpson`

// Overloaded `format`
logger::logf<logger::clog>(s2{"Homer", "Simpson"}, logger::format_type_short); // prints `Homer`
logger::logf<logger::clog>(s2{"Homer", "Simpson"}, logger::format_type_long); // prints `Homer Simpson`
```

## Установка

Библиотека не является header-only в строгом смысле, т.к. статические объекты `logger::logger_t::_instance` должны быть где-то определены.

Исходя из этого, существует три варианта подключения библиотеки:

1. Подключить весь проект как зависимость (для WinAPI-based проектов; конфликтует с MFC)
2. Добавить путь к директории `include` в пути поиска заголовочных файлов и вручную включить файл `logger.cpp` в проект (только если в проекте используются precompiled headers)
3. Добавить путь к директории `include` в пути поиска заголовочных файлов и вручную включить файл `<act-common/logger_impl.h>` в какой-либо специально отведенный `cpp`-файл (универсальный способ)