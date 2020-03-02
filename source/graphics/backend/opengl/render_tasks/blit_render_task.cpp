#include <magpie/graphics/backend/opengl/render_tasks/blit_render_task.hpp>

namespace mp
{
namespace opengl
{
fg::render_task<blit_task_data>* add_blit_render_task(renderer* framegraph, framebuffer_resource* source, framebuffer_resource* target)
{
  return framegraph->add_render_task<blit_task_data>(
    "Blit Pass",
    [&] (      blit_task_data& data, fg::render_task_builder& builder)
    {
      data.source = builder.read (source);
      data.target = builder.write(target);
    },
    [=] (const blit_task_data& data)
    {
      data.target->actual()->blit(
        *data.source->actual(),
        0, 
        0, 
        data.source->actual()->color_texture()->width (),
        data.source->actual()->color_texture()->height(),
        0, 
        0, 
        data.target->actual()->color_texture()->width (),
        data.target->actual()->color_texture()->height(),
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
      gl::print_error("Error in Blit Pass");
    });
}
}
}