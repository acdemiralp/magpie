#include <magpie/core/logger.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace mp
{
std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("console");
}