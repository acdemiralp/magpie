#ifndef MAGPIE_CORE_FRAME_TIMER_HPP
#define MAGPIE_CORE_FRAME_TIMER_HPP

#include <chrono>

#include <magpie/export.hpp>

namespace mp
{
// Frame timer keeps track of time and the delta between two consecutive calls to tick().
template<typename type, typename period>
class MAGPIE_EXPORT frame_timer_t
{
public:
  using clock      = std::chrono::high_resolution_clock       ;
  using duration   = std::chrono::duration<type, period>      ;
  using time_point = std::chrono::time_point <clock, duration>;

  frame_timer_t           () : delta_time_(0), time_(clock::now()) { }
  frame_timer_t           (const frame_timer_t&  that) = default;
  frame_timer_t           (      frame_timer_t&& temp) = default;
 ~frame_timer_t           ()                           = default;
  frame_timer_t& operator=(const frame_timer_t&  that) = default;
  frame_timer_t& operator=(      frame_timer_t&& temp) = default;

  void       tick      ()
  {
    time_point time = clock::now();
    delta_time_ = time - time_;
    time_       = time;
  }

  duration   delta_time() const
  {
    return delta_time_;
  }
  time_point time      () const
  {
    return time_;
  }

protected:
  duration   delta_time_;
  time_point time_      ;
};

using frame_timer = frame_timer_t<float, std::milli>;
}

#endif