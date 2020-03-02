#include <magpie/graphics/projection.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace mp
{
void                                                                                 projection::set_perspective (const float fov_y, const float aspect_ratio, std::array<float, 2> z_range)
{
  parameters_ = perspective {fov_y, aspect_ratio, z_range};
  matrix_     = glm::perspective(glm::radians(fov_y), aspect_ratio, z_range[0], z_range[1]);
}
void                                                                                 projection::set_orthographic(const di::rectangle<float> rectangle       , std::array<float, 2> z_range)
{
  parameters_ = orthographic {rectangle, z_range};
  matrix_     = glm::ortho(rectangle.left, rectangle.right, rectangle.bottom, rectangle.top, z_range[0], z_range[1]);
}
void                                                                                 projection::set_frustum     (const di::rectangle<float> rectangle       , std::array<float, 2> z_range)
{
  parameters_ = frustum {rectangle, z_range};
  matrix_     = glm::frustum(rectangle.left, rectangle.right, rectangle.bottom, rectangle.top, z_range[0], z_range[1]);
}
                                                                                     
enum projection::mode                                                                projection::mode            () const
{
  if (std::holds_alternative<perspective> (parameters_))
    return mode::perspective;
  if (std::holds_alternative<orthographic>(parameters_))
    return mode::orthographic;
  if (std::holds_alternative<frustum>     (parameters_))
    return mode::frustum;
  return mode::custom;
}
std::variant<projection::perspective, projection::orthographic, projection::frustum> projection::parameters      () const
{
  return parameters_;
}
const glm::mat4&                                                                     projection::matrix          () const
{
  return matrix_;
}
}
