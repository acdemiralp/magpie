#ifndef MAGPIE_CORE_LOGGER_HPP
#define MAGPIE_CORE_LOGGER_HPP

#include <memory>

#include <spdlog/logger.h>

#include <magpie/export.hpp>

namespace mp
{
MAGPIE_EXPORT extern std::shared_ptr<spdlog::logger> logger;
}

#endif