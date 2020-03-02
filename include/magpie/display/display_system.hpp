#ifndef MAGPIE_DISPLAY_DISPLAY_SYSTEM_HPP
#define MAGPIE_DISPLAY_DISPLAY_SYSTEM_HPP

#include <di/systems/display/display_system.hpp>

#include <magpie/core/system.hpp>
#include <magpie/export.hpp>

namespace mp
{
class MAGPIE_EXPORT display_system : public system, public di::display_system
{
public:
  void force_tick() { tick(); }

protected:
  void update(const frame_timer::duration& delta, scene* scene) override;
};
}

#endif