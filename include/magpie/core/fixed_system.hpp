#ifndef MAGPIE_CORE_FIXED_SYSTEM_HPP
#define MAGPIE_CORE_FIXED_SYSTEM_HPP

#include <magpie/core/system.hpp>
#include <magpie/export.hpp>

namespace mp
{
// A fixed system updates in fixed intervals.
class MAGPIE_EXPORT fixed_system : public system
{
public:
  explicit fixed_system  (const frame_timer::duration& interval);
  fixed_system           (const fixed_system&  that) = default;
  fixed_system           (      fixed_system&& temp) = default;
  virtual ~fixed_system  ()                          = default;
  fixed_system& operator=(const fixed_system&  that) = default;
  fixed_system& operator=(      fixed_system&& temp) = default;
  
  virtual void update(                                    scene* scene) = 0;
  void         update(const frame_timer::duration& delta, scene* scene) override;

protected:
  frame_timer::duration interval_;
  frame_timer::duration counter_ ;
};
}

#endif