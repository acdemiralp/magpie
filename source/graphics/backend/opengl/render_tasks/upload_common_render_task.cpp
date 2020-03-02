#include <magpie/graphics/backend/opengl/render_tasks/upload_common_render_task.hpp>

#include <gl/all.hpp>
#include <glm/glm.hpp>

#include <magpie/core/configuration.hpp>
#include <magpie/core/scene.hpp>

namespace mp
{
namespace opengl
{
struct glsl_camera
{
  glm::mat4  view      ;
  glm::mat4  projection;
};           
struct glsl_light
{            
  glm::uvec4 type      ; // y, z, w are unused.
  glm::vec4  color     ; // w is unused.
  glm::vec4  properties; // intensity - range - inner spot - outer spot
  glm::vec4  position  ; // w is unused.
  glm::vec4  direction ; // w is unused.
};

fg::render_task<upload_common_task_data>* add_upload_common_render_task(renderer* renderer)
{
  const auto retained_cameras       = renderer->add_retained_resource<buffer_description, gl::buffer>           ("Scene Cameras"      , buffer_description{configuration::instance().get<GLsizeiptr>("rendering.upload_common_render_task.camera_buffer_size", 16e+6)});
  const auto retained_lights        = renderer->add_retained_resource<buffer_description, gl::buffer>           ("Scene Lights"       , buffer_description{configuration::instance().get<GLsizeiptr>("rendering.upload_common_render_task.light_buffer_size" , 16e+6)});
  const auto retained_parameter_map = renderer->add_retained_resource<parameter_map::description, parameter_map>("Scene Parameter Map", parameter_map::description());

  return renderer->add_render_task<upload_common_task_data>(
    "Upload Common Data Pass",
    [=] (      upload_common_task_data& data, fg::render_task_builder& builder)
    {
      data.cameras       = builder.write<buffer_resource>       (retained_cameras      );
      data.lights        = builder.write<buffer_resource>       (retained_lights       );
      data.parameter_map = builder.write<parameter_map_resource>(retained_parameter_map);
    },
    [=] (const upload_common_task_data& data)
    {      
      auto scene = renderer->scene_cache();

      auto cameras = std::vector<glsl_camera>();
      for (auto& entity : scene->entities<transform, projection>())
      {
        auto metadata   = entity->component<mp::metadata>  ();
        auto transform  = entity->component<mp::transform> ();
        auto projection = entity->component<mp::projection>();
        if (!metadata->active) continue;
        cameras.push_back(glsl_camera
        {
          inverse(transform->matrix(true)),
          projection->matrix()
        });
      }
      auto cameras_metadata = glm::uvec4(cameras.size(), 0u, 0u, 0u);
      data.cameras      ->actual()->set_sub_data(0                      , sizeof cameras_metadata           , &cameras_metadata);
      data.cameras      ->actual()->set_sub_data(sizeof cameras_metadata, sizeof cameras[0] * cameras.size(), cameras.data()   );
      data.parameter_map->actual()->set         ("camera_count", cameras.size());

      gl::print_error("Error in Upload Common Data Pass");
    });
}
}
}
