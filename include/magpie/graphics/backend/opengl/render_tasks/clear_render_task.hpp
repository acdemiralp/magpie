#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_CLEAR_RENDER_TASK_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_CLEAR_RENDER_TASK_HPP

#include <glm/vec4.hpp>

#include <magpie/graphics/backend/opengl/framebuffer.hpp>
#include <magpie/graphics/renderer.hpp>
#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
struct MAGPIE_EXPORT clear_task_data
{
  framebuffer_resource* target;
};

MAGPIE_EXPORT fg::render_task<clear_task_data>* add_clear_render_task(renderer* framegraph, framebuffer_resource* target, const glm::vec4& color, const float depth = 1.0f);
}
}

#endif