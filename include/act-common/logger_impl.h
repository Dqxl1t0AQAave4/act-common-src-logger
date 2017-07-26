#pragma once

#include <act-common/logger.h>

template <> logger::clog logger::clog::_instance{};
template <> logger::wlog logger::wlog::_instance{};