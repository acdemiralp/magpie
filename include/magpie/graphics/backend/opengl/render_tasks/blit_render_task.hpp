#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_BLIT_RENDER_TASK_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_BLIT_RENDER_TASK_HPP

#include <magpie/graphics/backend/opengl/framebuffer.hpp>
#include <magpie/graphics/renderer.hpp>
#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
struct MAGPIE_EXPORT blit_task_data
{
  framebuffer_resource* source;
  framebuffer_resource* target;
};

MAGPIE_EXPORT fg::render_task<blit_task_data>* add_blit_render_task(renderer* framegraph, framebuffer_resource* source, framebuffer_resource* target);
}
}

#endif