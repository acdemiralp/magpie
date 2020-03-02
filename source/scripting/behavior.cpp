#include <magpie/scripting/behavior.hpp>

namespace mp
{
behavior::behavior(
  const std::function<void(                       scene*, entity*)>& on_prepare,
  const std::function<void(frame_timer::duration, scene*, entity*)>& on_update )
  : on_prepare_(on_prepare), on_update_(on_update)
{
  
}
}
