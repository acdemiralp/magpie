#ifndef MAKINA_CORE_SCENE_HPP
#define MAKINA_CORE_SCENE_HPP

#include <memory>
#include <vector>

#include <ec/entity.hpp>
#include <ec/scene.hpp>

#include <magpie/core/metadata.hpp>
#include <magpie/input/controller.hpp>
#include <magpie/graphics/projection.hpp>
#include <magpie/graphics/transform.hpp>

namespace mp
{
class behavior  ;
using behaviors = std::vector<std::shared_ptr<behavior>>;

using entity    = ec::entity< 
  // Common
  metadata      , 
  transform     ,
  // Rendering 
  projection    ,
  // Scripting
  behaviors     ,
  controller    >;
using scene     = ec::scene<entity>;

MAGPIE_EXPORT void append_scene(scene* source, scene* target);
MAGPIE_EXPORT void print_scene (const scene* scene);
}

#endif