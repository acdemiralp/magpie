#include <magpie/graphics/backend/glsl/immediate_point_fragment_shader.hpp>

namespace mp
{
namespace glsl
{
std::string immediate_point_fragment_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#endif

const float antialiasing = 2.0f;

layout(location = 0) in vs_output_type 
{
  noperspective float size ;
  vec4                color;
} fs_input;

layout(location = 0) out vec4 color;

void main()
{
  color    = fs_input.color;
  color.a *= smoothstep(0.5f, 0.5f - (antialiasing / fs_input.size), length(gl_PointCoord.xy - vec2(0.5f)));
  if (color.a <= 0.2f) discard;
}
)";
}
}