#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_VERTEX_ARRAY_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_VERTEX_ARRAY_HPP

#include <fg/resource.hpp>
#include <gl/vertex_array.hpp>

#include <magpie/graphics/backend/opengl/buffer.hpp>
#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
// A gl::vertex_array with support for recording shader storage and draw indirect buffers.
class MAGPIE_EXPORT vertex_array : public gl::vertex_array
{
public:
  struct MAGPIE_EXPORT attribute_binding
  {
    buffer_resource* buffer          ;
    GLint            components      ;
    GLenum           type            ;
    GLboolean        normalize       = GL_FALSE;
    GLuint           offset          = 0;
    GLsizei          stride          = 0;
    GLuint           relative_offset = 0;
    GLuint           divisor         = 0;
  };
  struct MAGPIE_EXPORT description
  {
    std::vector<attribute_binding> attribute_bindings     ;
    std::vector<buffer_resource*>  shader_storage_buffers ;
    buffer_resource*               element_buffer         = nullptr;
    buffer_resource*               draw_indirect_buffer   = nullptr;
  };

  explicit vertex_array  (const description& description);
  vertex_array           (const vertex_array&  that) = delete ;
  vertex_array           (      vertex_array&& temp) = default;
  virtual ~vertex_array  ()                          = default;
  vertex_array& operator=(const vertex_array&  that) = delete ;
  vertex_array& operator=(      vertex_array&& temp) = default;

  void bind  () const;
  void unbind() const;

protected:
  std::vector<buffer_resource*> shader_storage_buffers_;
  buffer_resource*              draw_indirect_buffer_  ;
};
using vertex_array_resource = fg::resource<vertex_array::description, vertex_array>;
}
}

template<>
inline std::unique_ptr<mp::opengl::vertex_array> fg::realize(const mp::opengl::vertex_array::description& description)
{
  return std::make_unique<mp::opengl::vertex_array>(description);
}

#endif