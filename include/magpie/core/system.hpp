#ifndef MAGPIE_CORE_SYSTEM_HPP
#define MAGPIE_CORE_SYSTEM_HPP

#include <magpie/core/frame_timer.hpp>
#include <magpie/core/scene_fwd.hpp>
#include <magpie/export.hpp>

namespace mp
{
class engine;

// A system is anything that updates regularly.
class MAGPIE_EXPORT system
{
public:
  system           ()                    = default;
  system           (const system&  that) = default;
  system           (      system&& temp) = default;
  virtual ~system  ()                    = default;
  system& operator=(const system&  that) = default;
  system& operator=(      system&& temp) = default;
  
  virtual void prepare(                                    scene* scene) { };
  virtual void update (const frame_timer::duration& delta, scene* scene) = 0;

protected:
  friend engine;

  engine* engine_ = nullptr;
};
}

#endif