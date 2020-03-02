#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_UPLOAD_COMMON_RENDER_TASK_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_UPLOAD_COMMON_RENDER_TASK_HPP

#include <fg/render_task.hpp>

#include <magpie/graphics/backend/opengl/buffer.hpp>
#include <magpie/graphics/parameter_map.hpp>
#include <magpie/graphics/renderer.hpp>
#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
struct MAGPIE_EXPORT upload_common_task_data
{
  buffer_resource*        cameras      ;
  buffer_resource*        lights       ;
  parameter_map_resource* parameter_map;
};

MAGPIE_EXPORT fg::render_task<upload_common_task_data>* add_upload_common_render_task(renderer* renderer);
}
}

#endif