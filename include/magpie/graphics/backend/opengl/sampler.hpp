#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_SAMPLER_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_SAMPLER_HPP

#include <fg/resource.hpp>
#include <gl/sampler.hpp>

#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
struct MAGPIE_EXPORT sampler_description
{
  std::array<GLenum, 3> wrap       = {GL_REPEAT, GL_REPEAT, GL_REPEAT};
  GLenum                min_filter = GL_NEAREST;
  GLenum                mag_filter = GL_NEAREST;
};
using sampler_resource = fg::resource<sampler_description, gl::sampler>;
}
}

template<>
inline std::unique_ptr<gl::sampler> fg::realize(const mp::opengl::sampler_description& description)
{
  auto   sampler = std::make_unique<gl::sampler>();
  sampler->set_wrap_s    (description.wrap[0]);
  sampler->set_wrap_t    (description.wrap[1]);
  sampler->set_wrap_r    (description.wrap[2]);
  sampler->set_min_filter(description.min_filter);
  sampler->set_mag_filter(description.mag_filter);
  return sampler;
}

#endif