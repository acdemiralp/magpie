#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_UI_RENDER_TASK_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_UI_RENDER_TASK_HPP

#include <magpie/graphics/backend/opengl/buffer.hpp>
#include <magpie/graphics/backend/opengl/framebuffer.hpp>
#include <magpie/graphics/backend/opengl/program.hpp>
#include <magpie/graphics/backend/opengl/texture.hpp>
#include <magpie/graphics/backend/opengl/vertex_array.hpp>
#include <magpie/graphics/renderer.hpp>
#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
struct MAGPIE_EXPORT ui_task_data
{
  buffer_resource*           attributes  ;
  buffer_resource*           indices     ;
  buffer_resource*           projection  ;
  texture_2d_resource*       texture     ;
                         
  graphics_program_resource* program     ;
  vertex_array_resource*     vertex_array;
  framebuffer_resource*      target      ;
};

MAGPIE_EXPORT fg::render_task<ui_task_data>* add_ui_render_task(renderer* framegraph, framebuffer_resource* target);
}
}

#endif