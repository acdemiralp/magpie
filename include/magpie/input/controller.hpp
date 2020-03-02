#ifndef MAGPIE_INPUT_CONTROLLER_HPP
#define MAGPIE_INPUT_CONTROLLER_HPP

#include <array>
#include <cstddef>
#include <functional>

#include <di/systems/input/key.hpp>

#include <magpie/export.hpp>

namespace mp
{
class transform;

struct MAGPIE_EXPORT controller
{
  std::function<void(transform*, di::key)>                     on_key_press    ;
  std::function<void(transform*, di::key)>                     on_key_down     ;
  std::function<void(transform*, di::key)>                     on_key_release  ;
  std::function<void(transform*, std::size_t)>                 on_mouse_press  ;
  std::function<void(transform*, std::size_t)>                 on_mouse_down   ;
  std::function<void(transform*, std::size_t)>                 on_mouse_release;
  std::function<void(transform*, std::array<std::int32_t, 2>)> on_mouse_move   ;
  std::function<void(transform*, std::array<std::int32_t, 2>)> on_mouse_wheel  ;
};
}

#endif