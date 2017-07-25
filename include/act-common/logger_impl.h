#pragma once

#include <act-common/logger.h>

template <> logger::log  logger::log::_instance{};
template <> logger::wlog logger::wlog::_instance{};