#include <magpie/graphics/backend/opengl/render_tasks.hpp>

#include <gl/all.hpp>

#include <magpie/core/engine.hpp>
#include <magpie/graphics/backend/opengl/framebuffer.hpp>
#include <magpie/graphics/renderer.hpp>
#include <magpie/input/input_system.hpp>

namespace mp
{
void opengl::make_default_framegraph       (engine* engine, di::window* window)
{
  gl::initialize();

  const auto input_system              = engine->system<mp::input_system>();
  const auto renderer                  = engine->system<mp::renderer>    ();

  const auto backbuffer                = renderer->add_retained_resource("Backbuffer", framebuffer::description(), default_framebuffer(window));

  const auto upload_common_render_task = add_upload_common_render_task(renderer);
  const auto clear_render_task         = add_clear_render_task        (renderer,               backbuffer, {0.0F, 0.0F, 0.0F, 0.0F});
  const auto immediate_render_task     = add_immediate_render_task    (renderer, input_system, backbuffer, upload_common_render_task->data());
  const auto ui_render_task            = add_ui_render_task           (renderer,               backbuffer);
}
}
