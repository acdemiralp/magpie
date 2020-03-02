#include <magpie/graphics/backend/glsl/immediate_triangle_vertex_shader.hpp>

namespace mp
{
namespace glsl
{
std::string immediate_triangle_vertex_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#endif

struct _camera
{
  mat4 view      ;
  mat4 projection;
};

layout(std430, binding = 0) readonly buffer camera
{
  uvec4     cameras_metadata  ; // x size, y index
  _camera   cameras[]         ;
};

layout(location = 0) in  vec4 attributes;
layout(location = 1) in  vec4 color     ;
layout(location = 0) out vs_output_type 
{
  vec4 color;
} vs_output;

void main()
{
  vs_output.color = color.abgr;
  gl_Position     = cameras[cameras_metadata.y].projection * cameras[cameras_metadata.y].view * vec4(attributes.xyz, 1.0f);
}
)";
}
}