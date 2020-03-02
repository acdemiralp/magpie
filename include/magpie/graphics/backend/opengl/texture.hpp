#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_TEXTURE_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_TEXTURE_HPP

#include <fg/resource.hpp>
#include <gl/texture.hpp>

#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
struct MAGPIE_EXPORT texture_description
{
  std::array<GLsizei, 3> size   ;
  GLenum                 format = GL_RGBA8;
  GLsizei                levels = 1       ;
};
using texture_1d_resource       = fg::resource<texture_description, gl::texture_1d>;
using texture_2d_resource       = fg::resource<texture_description, gl::texture_2d>;
using texture_3d_resource       = fg::resource<texture_description, gl::texture_3d>;
using texture_1d_array_resource = fg::resource<texture_description, gl::texture_1d_array>;
using texture_2d_array_resource = fg::resource<texture_description, gl::texture_2d_array>;
}
}

template<>
inline std::unique_ptr<gl::texture_1d>       fg::realize(const mp::opengl::texture_description& description)
{
  auto   texture = std::make_unique<gl::texture_1d>();
  texture->set_storage(description.levels, description.format, description.size[0]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_2d>       fg::realize(const mp::opengl::texture_description& description)
{
  auto   texture = std::make_unique<gl::texture_2d>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_3d>       fg::realize(const mp::opengl::texture_description& description)
{
  auto   texture = std::make_unique<gl::texture_3d>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1], description.size[2]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_1d_array> fg::realize(const mp::opengl::texture_description& description)
{
  auto   texture = std::make_unique<gl::texture_1d_array>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_2d_array> fg::realize(const mp::opengl::texture_description& description)
{
  auto   texture = std::make_unique<gl::texture_2d_array>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1], description.size[2]);
  return texture;
}

#endif