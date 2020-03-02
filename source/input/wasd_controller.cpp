#include <magpie/input/wasd_controller.hpp>

#include <di/systems/input/mouse.hpp>
#include <glm/vec3.hpp>

#include <magpie/graphics/transform.hpp>

namespace mp
{
controller make_wasd_controller(const float move_sensitivity, const float look_sensitivity)
{
  controller wasd_controller;
  wasd_controller.on_key_down   = [=] (transform* transform, di::key key)
  {
    if(key.code == di::key_code::w) transform->translate(transform->forward() *  move_sensitivity);
    if(key.code == di::key_code::s) transform->translate(transform->forward() * -move_sensitivity);
    if(key.code == di::key_code::a) transform->translate(transform->right  () * -move_sensitivity);
    if(key.code == di::key_code::d) transform->translate(transform->right  () *  move_sensitivity);
  };
  wasd_controller.on_mouse_move = [=] (transform* transform, std::array<std::int32_t, 2> value)
  {
    di::mouse::set_captured(true);
    transform->rotate(angleAxis(glm::radians(value[0] * look_sensitivity), glm::vec3(0, 1, 0)) * 
                      angleAxis(glm::radians(value[1] * look_sensitivity), transform->right()));
  };
  return wasd_controller;
}
}
