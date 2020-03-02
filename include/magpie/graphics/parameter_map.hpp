#ifndef MAGPIE_GRAPHICS_PARAMETER_MAP_HPP
#define MAGPIE_GRAPHICS_PARAMETER_MAP_HPP

#include <map>

#include <boost/lexical_cast.hpp>
#include <fg/resource.hpp>

#include <magpie/export.hpp>

namespace mp
{
// Utility resource for passing data between render tasks live, during traversal.
class MAGPIE_EXPORT parameter_map
{
public:
  class MAGPIE_EXPORT description { };

  template<typename type>
  void set(const std::string& key, const type& value)
  {
    map_[key] = boost::lexical_cast<std::string>(value);
  }
  template<typename type>
  type get(const std::string& key) const
  {
    return map_.count(key) ? boost::lexical_cast<type>(map_.at(key)) : type();
  }

protected:
  std::map<std::string, std::string> map_;
};

using parameter_map_resource = fg::resource<parameter_map::description, parameter_map>;
}

template<>
inline std::unique_ptr<mp::parameter_map> fg::realize(const mp::parameter_map::description& description)
{
  return std::make_unique<mp::parameter_map>();
}

#endif