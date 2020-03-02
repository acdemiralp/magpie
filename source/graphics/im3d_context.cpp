#include <magpie/graphics/im3d_context.hpp>

namespace mp
{
Im3d::Context& im3d_context()
{
  return Im3d::GetContext();
}
}
