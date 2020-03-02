#ifndef MAGPIE_CORE_METADATA_HPP
#define MAGPIE_CORE_METADATA_HPP

#include <algorithm>
#include <string>
#include <vector>

#include <magpie/core/scene_fwd.hpp>

namespace mp
{
struct metadata
{
  bool contains_tag(const std::string& tag)
  {
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
  }
  
  entity*                  entity ;
  std::string              name   ;
  std::vector<std::string> tags   ;
  bool                     active = true;
};
}

#endif