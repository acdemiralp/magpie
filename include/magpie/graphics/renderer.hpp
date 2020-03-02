#ifndef MAGPIE_GRAPHICS_RENDERER_HPP
#define MAGPIE_GRAPHICS_RENDERER_HPP

#include <fg/framegraph.hpp>

#include <magpie/core/system.hpp>
#include <magpie/export.hpp>

namespace mp
{
// Renderer is a framegraph system.
class MAGPIE_EXPORT renderer : public fg::framegraph, public system
{
public:
  const frame_timer::duration& delta_cache() const;
  const scene*                 scene_cache() const;

protected:
  void prepare(                                    scene* scene) override;
  void update (const frame_timer::duration& delta, scene* scene) override;

  frame_timer::duration delta_cache_ = {};
  scene*                scene_cache_ = nullptr;
};
}

#endif