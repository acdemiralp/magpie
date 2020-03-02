#ifndef MAGPIE_GRAPHICS_IMGUI_CONTEXT_HPP
#define MAGPIE_GRAPHICS_IMGUI_CONTEXT_HPP

#include <imgui.h>

#include <magpie/export.hpp>

namespace mp
{
MAGPIE_EXPORT ImGuiContext* imgui_context    ();

MAGPIE_EXPORT void          imgui_begin_scene();
MAGPIE_EXPORT void          imgui_end_scene  (); 
}

#endif