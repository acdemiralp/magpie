#include <magpie/default_engine.hpp>

#include <magpie/display/display_system.hpp>
#include <magpie/graphics/renderer.hpp>
#include <magpie/graphics/ui_system.hpp>
#include <magpie/input/input_system.hpp>
#include <magpie/input/wasd_controller.hpp>
#include <magpie/scripting/scripting_system.hpp>

namespace mp
{
std::unique_ptr<engine> make_default_engine()
{
  auto engine            = std::make_unique<mp::engine>();
  auto engine_ptr        = engine.get();
  auto display_system    = engine->add_system<mp::display_system>   ();
  auto input_system      = engine->add_system<mp::input_system>     ();
  auto ui_system         = engine->add_system<mp::ui_system>        (display_system, input_system);
  auto scripting_system  = engine->add_system<mp::scripting_system> ();
  auto renderer          = engine->add_system<mp::renderer>         ();

  input_system->on_quit     .connect([engine_ptr] ()
  {
    engine_ptr->stop();
  });

  auto scene = std::make_unique<mp::scene>();
  {
    auto entity          = scene ->add_entity();
    auto metadata        = entity->add_component<mp::metadata>      ();
    auto transform       = entity->add_component<mp::transform>     ();
    auto projection      = entity->add_component<mp::projection>    ();
    auto controller      = entity->add_component<mp::controller>    (make_wasd_controller());
    metadata  ->entity   = entity;
    metadata  ->name     = "Default Camera";
    metadata  ->tags.push_back("default_camera");
    transform ->set_translation(glm::vec3(0.0f, 0.0f, -10.0f));
    projection->set_perspective(60.0f, 4.0f / 3.0f, {0.3f, 1000.0f});
    
    input_system->on_key_press.connect([=] (di::key key)
    {
      if (key.code == di::key_code::left_alt || key.code == di::key_code::right_alt)
      {
        di::mouse::set_visible ( di::mouse::relative());
        di::mouse::set_relative(!di::mouse::relative());
        metadata->active = di::mouse::relative();
      }
    });
    di::mouse::set_visible (false);
    di::mouse::set_relative(true );
  }
  engine->set_scene(std::move(scene));

  return engine;
}
}
