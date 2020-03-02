#include <magpie/core/scene.hpp>

#include <magpie/core/logger.hpp>
#include <magpie/graphics/transform.hpp>

namespace mp
{
void append_scene(scene* source, scene* target)
{
  auto source_entities = source->entities();
  auto target_entities = std::vector<entity*>();
  
  const std::function<void(entity*, entity*)> recursive_add_entity = [&] (mp::entity* source_entity, mp::entity* parent)
  {
    auto entity       = target->copy_entity(source_entity);
    auto transform    = entity->component<mp::transform>();
    auto metadata     = entity->component<mp::metadata> ();
    metadata ->entity = entity;
    transform->set_children({});
    if (parent) transform->set_parent(parent->component<mp::transform>());

    target_entities.push_back(entity);

    for (auto child : source_entity->component<mp::transform>()->children())
      recursive_add_entity(*std::find_if(
        source_entities.begin(),
        source_entities.end  (),
        [&] (mp::entity* iteratee)
        {
          return child == iteratee->component<mp::transform>();
        }), 
        entity);
  };

  for (auto& entity : source_entities)
    if (!entity->component<transform>()->parent())
      recursive_add_entity(entity, nullptr);
}
void print_scene (const scene* scene)
{
  const std::function<void(entity*, std::size_t)> recursive_print = [&] (entity* entity, const std::size_t depth)
  {
    if (entity->has_components<metadata>())
      logger->info("{}- {} ({})", depth > 0 ? std::string(depth, ' ') : "", entity->component<metadata>()->name, entity->bitset().to_string());

    auto entities = scene ->entities <transform>();
    auto children = entity->component<transform>()->children();
    for (auto child : children)
      recursive_print(*std::find_if(entities.begin(), entities.end(), [&] (mp::entity* iteratee) {return iteratee->component<transform>() == child;}), depth + 1);
  };

  logger->info("{}", std::string(50, '#'));

  logger->info("Transformless Entities");
  for (auto entity : scene->entities())
    if (!entity->has_components<transform>() && entity->has_components<metadata>())
      logger->info("- {} ({})", entity->component<metadata>()->name, entity->bitset().to_string());

  logger->info("{}", std::string(50, '#'));

  logger->info("Transform Hierarchy");
  for (auto entity : scene->entities())
    if ( entity->has_components<transform>() && !entity->component<transform>()->parent())
      recursive_print(entity, 0);

  logger->info("{}", std::string(50, '#'));
}
}