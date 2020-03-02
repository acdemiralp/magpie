#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_RENDER_TASKS_HPP

#include <magpie/graphics/backend/opengl/render_tasks/blit_render_task.hpp>
#include <magpie/graphics/backend/opengl/render_tasks/clear_render_task.hpp>
#include <magpie/graphics/backend/opengl/render_tasks/immediate_render_task.hpp>
#include <magpie/graphics/backend/opengl/render_tasks/ui_render_task.hpp>
#include <magpie/export.hpp>

namespace di
{
class window;
}

namespace mp
{
class engine;

namespace opengl
{
MAGPIE_EXPORT void make_default_framegraph(engine* engine, di::window* window);
}
}

#endif