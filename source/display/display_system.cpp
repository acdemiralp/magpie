#include <magpie/display/display_system.hpp>

namespace mp
{
void display_system::update(const frame_timer::duration& delta, scene* scene)
{
  tick();
}
}
