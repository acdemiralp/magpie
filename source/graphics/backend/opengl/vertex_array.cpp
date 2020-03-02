#include <magpie/graphics/backend/opengl/vertex_array.hpp>

#include <cstddef>

namespace mp
{
namespace opengl
{
vertex_array::vertex_array(const description& description)
{
  // Important: Vertex arrays must be realized after all of the referred buffers are realized.
  for(GLuint i = 0; i < static_cast<GLuint>(description.attribute_bindings.size()); ++i)
  {
    auto& binding = description.attribute_bindings[i];
    set_vertex_buffer    (i, *binding.buffer->actual(), binding.offset, binding.stride != 0u ? binding.stride : binding.components * gl::type_size(binding.type));
    set_attribute_enabled(i, true);

    if(!binding.normalize && (
       binding.type == GL_BYTE  || binding.type == GL_UNSIGNED_BYTE  ||
       binding.type == GL_SHORT || binding.type == GL_UNSIGNED_SHORT ||
       binding.type == GL_INT   || binding.type == GL_UNSIGNED_INT   || binding.type == GL_FIXED))
      set_attribute_format_integer(i, binding.components, binding.type, binding.relative_offset);
    else if(binding.type == GL_DOUBLE)
      set_attribute_format_long   (i, binding.components, binding.type, binding.relative_offset);
    else // if(binding.type == GL_FLOAT || binding.type == GL_HALF_FLOAT)
      set_attribute_format        (i, binding.components, binding.type, binding.normalize, binding.relative_offset);

    set_binding_divisor(i, binding.divisor);
  }

  if (description.element_buffer)
    set_element_buffer(*description.element_buffer->actual());
  
  shader_storage_buffers_ = description.shader_storage_buffers;
  draw_indirect_buffer_   = description.draw_indirect_buffer  ;
}

void vertex_array::bind  () const
{
  gl::vertex_array::bind  ();
  for (std::size_t i = 0; i < shader_storage_buffers_.size(); ++i)
    shader_storage_buffers_[i]->actual()->bind_base  (GL_SHADER_STORAGE_BUFFER, static_cast<GLuint>(i));
  if (draw_indirect_buffer_)
      draw_indirect_buffer_->actual()->bind  (GL_DRAW_INDIRECT_BUFFER);
}
void vertex_array::unbind() const
{
  if (draw_indirect_buffer_)
      draw_indirect_buffer_->actual()->unbind(GL_DRAW_INDIRECT_BUFFER);
  for (std::size_t i = 0; i < shader_storage_buffers_.size(); ++i)
    shader_storage_buffers_[i]->actual()->unbind_base(GL_SHADER_STORAGE_BUFFER, static_cast<GLuint>(i));
  gl::vertex_array::unbind();
}
}
}
