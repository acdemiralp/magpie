#include <magpie/graphics/backend/glsl/immediate_triangle_fragment_shader.hpp>

namespace mp
{
namespace glsl
{
std::string immediate_triangle_fragment_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#endif

layout(location = 0) in vs_output_type 
{
  vec4 color;
} fs_input;

layout(location = 0) out vec4 color;

void main()
{
  color = fs_input.color;
}
)";
}
}