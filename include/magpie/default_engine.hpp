#ifndef MAGPIE_DEFAULT_ENGINE_HPP
#define MAGPIE_DEFAULT_ENGINE_HPP

#include <memory>

#include <magpie/core/engine.hpp>
#include <magpie/export.hpp>

namespace mp
{
MAGPIE_EXPORT std::unique_ptr<engine> make_default_engine();
}
#endif