#ifndef MAGPIE_SCRIPTING_BEHAVIOR_HPP
#define MAGPIE_SCRIPTING_BEHAVIOR_HPP

#include <functional>

#include <magpie/core/frame_timer.hpp>
#include <magpie/core/scene_fwd.hpp>
#include <magpie/export.hpp>

namespace mp
{
class scripting_system;

class MAGPIE_EXPORT behavior
{
public:
  explicit behavior  (
    const std::function<void(                       scene*, entity*)>& on_prepare,
    const std::function<void(frame_timer::duration, scene*, entity*)>& on_update );
  behavior           (const behavior&  that) = default;
  behavior           (      behavior&& temp) = default;
  virtual ~behavior  ()                      = default;
  behavior& operator=(const behavior&  that) = default;
  behavior& operator=(      behavior&& temp) = default;

protected:
  friend scripting_system;

  std::function<void(                       scene*, entity*)> on_prepare_;
  std::function<void(frame_timer::duration, scene*, entity*)> on_update_ ;
};
}

#endif