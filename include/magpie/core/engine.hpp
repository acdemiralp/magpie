#ifndef MAGPIE_CORE_ENGINE_HPP
#define MAGPIE_CORE_ENGINE_HPP

#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>

#include <magpie/core/frame_timer.hpp>
#include <magpie/core/logger.hpp>
#include <magpie/core/scene.hpp>
#include <magpie/core/system.hpp>
#include <magpie/export.hpp>

namespace mp
{
class MAGPIE_EXPORT engine
{
public:
  engine           ();
  engine           (const engine&  that) = delete ;
  engine           (      engine&& temp) = delete ;
  virtual ~engine  ()                    = default;
  engine& operator=(const engine&  that) = delete ;
  engine& operator=(      engine&& temp) = delete ;

  template<typename system_type, typename... system_arguments>
  system_type* add_system           (system_arguments&&... arguments)
  {
    static_assert(std::is_base_of<mp::system, system_type>::value, "The type does not inherit from system.");
    systems_.push_back(std::make_unique<system_type>(arguments...));
    const auto system = systems_.back().get();
    system->engine_ = this;
    return static_cast<system_type*>(system);
  }
  template<typename system_type>
  system_type* system               ()
  {
    static_assert(std::is_base_of<mp::system, system_type>::value, "The type does not inherit from system.");
    auto iterator = std::find_if(systems_.begin(), systems_.end(), system_type_predicate<system_type>);
    if  (iterator == systems_.end())
    {
      logger->warn("System does not exist within the engine.");
      return nullptr;
    }
    return static_cast<system_type*>(iterator->get());
  }
  template<typename system_type>
  void         remove_system        ()
  {
    static_assert(std::is_base_of<mp::system, system_type>::value, "The type does not inherit from system.");
    auto iterator = std::remove_if(systems_.begin(), systems_.end(), system_type_predicate<system_type>);
    if  (iterator == systems_.end())
    {
      logger->warn("System does not exist within the engine.");
      return;
    }
    systems_.erase(iterator, systems_.end());
  }
  
  scene*       scene                () const;
  void         set_scene            (std::unique_ptr<mp::scene> scene);

  void         run                  ();
  void         stop                 ();
  bool         is_running           () const;

protected:
  template<typename system_type>
  static bool  system_type_predicate(const std::unique_ptr<mp::system>& iteratee)
  {
    return typeid(system_type) == typeid(*iteratee.get());
  }

  std::vector<std::unique_ptr<mp::system>> systems_    ;
  std::unique_ptr<mp::scene>               scene_      ;
  frame_timer                              frame_timer_;
  bool                                     is_running_ = false;
};
}

#endif