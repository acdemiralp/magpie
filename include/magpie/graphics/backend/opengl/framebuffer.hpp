#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_FRAMEBUFFER_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_FRAMEBUFFER_HPP

#include <array>
#include <memory>

#include <di/systems/display/window.hpp>
#include <fg/resource.hpp>
#include <gl/framebuffer.hpp>
#include <gl/texture.hpp>

#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
// A gl::framebuffer with in-built color and depth textures.
class MAGPIE_EXPORT framebuffer : public gl::framebuffer
{
public:
  struct MAGPIE_EXPORT description
  {
    std::array<GLsizei, 2> size;
    GLenum                 color_format = GL_RGBA8;
    GLenum                 depth_format = GL_DEPTH_COMPONENT24;
  };

  explicit framebuffer  (di::window* window); // Default framebuffer constructor.
  explicit framebuffer  (const description&  description);
  framebuffer           (const framebuffer&  that) = delete ;
  framebuffer           (      framebuffer&& temp) = default;
  virtual ~framebuffer()                           = default;
  framebuffer& operator=(const framebuffer&  that) = delete ;
  framebuffer& operator=(      framebuffer&& temp) = default;
  
  gl::texture_2d* color_texture();
  gl::texture_2d* depth_texture();

  bool is_default() const;

protected:
  gl::texture_2d color_texture_;
  gl::texture_2d depth_texture_;
};

MAGPIE_EXPORT framebuffer* default_framebuffer(di::window* window);

using framebuffer_resource = fg::resource<framebuffer::description, framebuffer>;
}
}

template<>
inline std::unique_ptr<mp::opengl::framebuffer> fg::realize(const mp::opengl::framebuffer::description& description)
{
  return std::make_unique<mp::opengl::framebuffer>(description);
}

#endif