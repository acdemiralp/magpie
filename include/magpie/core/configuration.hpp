#ifndef MAGPIE_CORE_CONFIGURATION_HPP
#define MAGPIE_CORE_CONFIGURATION_HPP

#include <acd/singleton.hpp>
#include <nlohmann/json.hpp>

#include <magpie/export.hpp>

namespace mp
{
class MAGPIE_EXPORT configuration : public acd::singleton<configuration>
{
public:
  explicit configuration  (std::string filepath = "config.json");
  configuration           (const configuration&  that) = delete;
  configuration           (      configuration&& temp) = delete;
  virtual ~configuration  ();
  configuration& operator=(const configuration&  that) = delete;
  configuration& operator=(      configuration&& temp) = delete;

  template<typename type>
  type get(const std::string& key, const type default_value = type())
  {
    if (json_.find(key) == json_.end()) json_[key] = default_value;
    return json_[key].get<type>();
  }
  template<typename type>
  void set(const std::string& key, const type value)
  {
    json_[key] = value;
  }
  
protected:
  std::string    filepath_;
  nlohmann::json json_    ;
};
}

#endif