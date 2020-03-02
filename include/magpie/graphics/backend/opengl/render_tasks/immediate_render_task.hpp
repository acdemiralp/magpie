#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_IMMEDIATE_RENDER_TASK_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_IMMEDIATE_RENDER_TASK_HPP

#include <functional>
#include <string>

#include <im3d.h>

#include <magpie/input/input_system.hpp>
#include <magpie/graphics/backend/opengl/render_tasks/upload_common_render_task.hpp>
#include <magpie/graphics/backend/opengl/buffer.hpp>
#include <magpie/graphics/backend/opengl/framebuffer.hpp>
#include <magpie/graphics/backend/opengl/program.hpp>
#include <magpie/graphics/backend/opengl/vertex_array.hpp>
#include <magpie/graphics/renderer.hpp>
#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
struct MAGPIE_EXPORT immediate_task_data
{
  buffer_resource*                           attributes       ;
  buffer_resource*                           cameras          ;
  buffer_resource*                           viewport         ;
                                             
  graphics_program_resource*                 points_program   ;
  graphics_program_resource*                 lines_program    ;
  graphics_program_resource*                 triangles_program;
  vertex_array_resource*                     vertex_array     ;
  framebuffer_resource*                      target           ;

  std::function<void(const Im3d::DrawList&)> draw_callback    ;
};

MAGPIE_EXPORT fg::render_task<immediate_task_data>* add_immediate_render_task(
  renderer*                      framegraph  , 
  input_system*                  input_system, 
  framebuffer_resource*          target      , 
  const upload_common_task_data& common_data , 
  const std::string&             camera_tag  = std::string());
}
}

#endif