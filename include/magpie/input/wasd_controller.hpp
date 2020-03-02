#ifndef MAGPIE_INPUT_WASD_CONTROLLER_HPP
#define MAGPIE_INPUT_WASD_CONTROLLER_HPP

#include <magpie/input/controller.hpp>
#include <magpie/export.hpp>

namespace mp
{
MAGPIE_EXPORT controller make_wasd_controller(const float move_sensitivity = 1.0f, const float look_sensitivity = 0.25f);
}

#endif