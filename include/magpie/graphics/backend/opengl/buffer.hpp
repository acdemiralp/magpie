#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_BUFFER_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_BUFFER_HPP

#include <fg/resource.hpp>
#include <gl/buffer.hpp>

#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
struct MAGPIE_EXPORT buffer_description
{
  GLsizeiptr size  ;
  bool       clear = false;
};
using buffer_resource = fg::resource<buffer_description, gl::buffer>;
}
}

template<>
inline std::unique_ptr<gl::buffer> fg::realize(const mp::opengl::buffer_description& description)
{
  auto buffer = std::make_unique<gl::buffer>();
  buffer->set_data_immutable(description.size);
  if (description.clear)
  {
    std::uint8_t clear_value = 0;
    buffer->clear_sub_data(GL_R8, 0, description.size, GL_RED, GL_UNSIGNED_BYTE, &clear_value);
  }
  return buffer;
}

#endif