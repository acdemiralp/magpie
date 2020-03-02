#ifndef MAGPIE_GRAPHICS_BACKEND_OPENGL_TRANSFORM_FEEDBACK_HPP
#define MAGPIE_GRAPHICS_BACKEND_OPENGL_TRANSFORM_FEEDBACK_HPP

#include <fg/resource.hpp>
#include <gl/transform_feedback.hpp>

#include <magpie/graphics/backend/opengl/buffer.hpp>
#include <magpie/export.hpp>

namespace mp
{
namespace opengl
{
struct MAGPIE_EXPORT transform_feedback_description
{
  std::vector<buffer_resource*> buffers;
};
using transform_feedback_resource = fg::resource<transform_feedback_description, gl::transform_feedback>;
}
}

template<>
inline std::unique_ptr<gl::transform_feedback> fg::realize(const mp::opengl::transform_feedback_description& description)
{
  auto   transform_feedback = std::make_unique<gl::transform_feedback>();
  for(std::size_t i = 0; i < description.buffers.size(); ++i)
    transform_feedback->set_buffer_base(i, *description.buffers[i]->actual());
  return transform_feedback;
}

#endif