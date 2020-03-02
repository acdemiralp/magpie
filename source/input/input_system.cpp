#include <magpie/input/input_system.hpp>

#include <magpie/core/scene.hpp>
#include <magpie/input/controller.hpp>

namespace mp
{
input_system::input_system()
{
  on_key_press    .connect([&] (di::key     key   ) { key_states_   [key]    = true ; });
  on_key_release  .connect([&] (di::key     key   ) { key_states_   [key]    = false; });
  on_mouse_press  .connect([&] (std::size_t button) { button_states_[button] = true ; });
  on_mouse_release.connect([&] (std::size_t button) { button_states_[button] = false; });
}

void input_system::prepare(scene* scene)
{
  controller_cache_.clear();
}
void input_system::update (const frame_timer::duration& delta, scene* scene)
{
  tick();

  for (auto& state : key_states_   )
    if (state.second) on_key_down  (state.first);
  for (auto& state : button_states_)
    if (state.second) on_mouse_down(state.first);

  auto entities = scene->entities<controller, transform>();
  for(auto& entity : entities)
  {
    auto       controller = entity->component<mp::controller>();
    const auto metadata   = entity->component<mp::metadata>  ();
    const auto transform  = entity->component<mp::transform> ();
    if (std::find(controller_cache_.begin(), controller_cache_.end(), controller) == controller_cache_.end())
    {
      if(controller->on_key_press    ) on_key_press       .connect([=] (di::key                     key   ) { if(metadata->active) controller->on_key_press    (transform, key   ); });
      if(controller->on_key_down     ) on_key_down        .connect([=] (di::key                     key   ) { if(metadata->active) controller->on_key_down     (transform, key   ); });
      if(controller->on_key_release  ) on_key_release     .connect([=] (di::key                     key   ) { if(metadata->active) controller->on_key_release  (transform, key   ); });
      if(controller->on_mouse_press  ) on_mouse_press     .connect([=] (std::size_t                 button) { if(metadata->active) controller->on_mouse_press  (transform, button); });
      if(controller->on_mouse_down   ) on_mouse_down      .connect([=] (std::size_t                 button) { if(metadata->active) controller->on_mouse_down   (transform, button); });
      if(controller->on_mouse_release) on_mouse_release   .connect([=] (std::size_t                 button) { if(metadata->active) controller->on_mouse_release(transform, button); });
      if(controller->on_mouse_move   ) on_mouse_move_delta.connect([=] (std::array<std::int32_t, 2> value ) { if(metadata->active) controller->on_mouse_move   (transform, value ); });
      if(controller->on_mouse_wheel  ) on_mouse_wheel     .connect([=] (std::array<std::int32_t, 2> value ) { if(metadata->active) controller->on_mouse_wheel  (transform, value ); });
      controller_cache_.push_back(controller);
    }
  }
}
}
