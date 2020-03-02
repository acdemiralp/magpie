#ifndef MAGPIE_CORE_SCENE_FWD_HPP
#define MAGPIE_CORE_SCENE_FWD_HPP

#include <memory>
#include <vector>

namespace ec
{
template<typename... types>
class entity;

template<typename type>
class scene;
}

namespace mp
{
class  behavior  ;
struct metadata  ;
struct controller;
class  projection;
class  transform ;

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
}

#endif