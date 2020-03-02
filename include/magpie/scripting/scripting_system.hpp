#ifndef MAGPIE_SCRIPTING_SCRIPTING_SYSTEM_HPP
#define MAGPIE_SCRIPTING_SCRIPTING_SYSTEM_HPP

#include <magpie/core/scene_fwd.hpp>
#include <magpie/core/system.hpp>
#include <magpie/export.hpp>

namespace mp
{
class MAGPIE_EXPORT scripting_system : public system
{
protected:
  void prepare(                                    scene* scene) override;
  void update (const frame_timer::duration& delta, scene* scene) override;
};
}

#endif