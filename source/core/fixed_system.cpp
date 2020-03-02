#include <magpie/core/fixed_system.hpp>

namespace mp
{
fixed_system::fixed_system(const frame_timer::duration& interval) : interval_(interval), counter_(0)
{

}

void fixed_system::update(const frame_timer::duration& delta, scene* scene)
{
  counter_ += delta;
  if (counter_ >= interval_)
  {
    counter_ -= interval_;
    update(scene);
  }
}
}
