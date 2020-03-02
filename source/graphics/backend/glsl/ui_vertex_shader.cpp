#include <magpie/graphics/backend/glsl/ui_vertex_shader.hpp>

namespace mp
{
namespace glsl
{
std::string ui_vertex_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#endif

layout(std430, binding = 0) readonly buffer _projection
{
  mat4 projection;
};

layout(location = 0) in vec2 vertex            ;
layout(location = 1) in vec2 texture_coordinate;
layout(location = 2) in vec4 color             ;

layout(location = 0) out vs_output_type 
{
  vec2 texture_coordinate;
  vec4 color             ;
} vs_output;

#ifdef VULKAN
out gl_PerVertex
{
  vec4 gl_Position;
};
#endif

void main()
{
  vs_output.texture_coordinate = texture_coordinate;
  vs_output.color              = color             ;
  gl_Position                  = projection * vec4(vertex, 0.0f, 1.0f);
}
)";
}
}