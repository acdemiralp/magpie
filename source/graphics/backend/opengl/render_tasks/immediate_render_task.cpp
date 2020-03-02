#include <magpie/graphics/backend/opengl/render_tasks/immediate_render_task.hpp>

#include <di/systems/input/mouse.hpp>
#include <gl/draw_commands.hpp>
#include <gl/per_fragment_ops.hpp>
#include <gl/rasterization.hpp>
#include <gl/viewport.hpp>
#include <glm/glm.hpp>
#include <im3d.h>
#include <im3d_math.h>

#include <magpie/core/configuration.hpp>
#include <magpie/core/logger.hpp>
#include <magpie/core/scene.hpp>
#include <magpie/graphics/backend/glsl/immediate_line_fragment_shader.hpp>
#include <magpie/graphics/backend/glsl/immediate_line_geometry_shader.hpp>
#include <magpie/graphics/backend/glsl/immediate_line_vertex_shader.hpp>
#include <magpie/graphics/backend/glsl/immediate_point_fragment_shader.hpp>
#include <magpie/graphics/backend/glsl/immediate_point_vertex_shader.hpp>
#include <magpie/graphics/backend/glsl/immediate_triangle_fragment_shader.hpp>
#include <magpie/graphics/backend/glsl/immediate_triangle_vertex_shader.hpp>

namespace mp
{
namespace opengl
{
fg::render_task<immediate_task_data>* add_immediate_render_task (
  renderer*                      framegraph  , 
  input_system*                  input_system, 
  framebuffer_resource*          target      , 
  const upload_common_task_data& common_data , 
  const std::string&             camera_tag  )
{
  const auto retained_attributes        = framegraph->add_retained_resource<buffer_description, gl::buffer>                      ("Immediate Vertices"         , buffer_description{configuration::instance().get<GLsizeiptr>("rendering.immediate_render_task.attribute_buffer_size", 256e+6)});
  const auto retained_viewport          = framegraph->add_retained_resource<buffer_description, gl::buffer>                      ("Immediate Viewport"         , buffer_description{sizeof(glm::vec2)});
  const auto retained_points_program    = framegraph->add_retained_resource<graphics_program_resource::description_type, program>("Immediate Points Program"   , program::graphics_description
  {                                                                                                                                                            
    glsl::immediate_point_vertex_shader,                                                                                                                      
    glsl::immediate_point_fragment_shader                                                                                                                     
  });                                                                                                                                                          
  const auto retained_lines_program     = framegraph->add_retained_resource<graphics_program_resource::description_type, program>("Immediate Lines Program"    , program::graphics_description
  {
    glsl::immediate_line_vertex_shader,
    glsl::immediate_line_fragment_shader,
    glsl::immediate_line_geometry_shader
  });
  const auto retained_triangles_program = framegraph->add_retained_resource<graphics_program_resource::description_type, program>("Immediate Triangles Program", program::graphics_description
  {
    glsl::immediate_triangle_vertex_shader,
    glsl::immediate_triangle_fragment_shader
  });
  
  input_system->on_key_press  .connect([ ] (di::key key)
  {
    auto& app_data = Im3d::GetAppData();
    if      (key.code == di::key_code::l) app_data.m_keyDown[Im3d::Key_L] = true;
    else if (key.code == di::key_code::t) app_data.m_keyDown[Im3d::Key_T] = true;
    else if (key.code == di::key_code::r) app_data.m_keyDown[Im3d::Key_R] = true;
    else if (key.code == di::key_code::s) app_data.m_keyDown[Im3d::Key_S] = true;
  });
  input_system->on_key_release.connect([ ] (di::key key)
  {
    auto& app_data = Im3d::GetAppData();
    if      (key.code == di::key_code::l) app_data.m_keyDown[Im3d::Key_L] = false;
    else if (key.code == di::key_code::t) app_data.m_keyDown[Im3d::Key_T] = false;
    else if (key.code == di::key_code::r) app_data.m_keyDown[Im3d::Key_R] = false;
    else if (key.code == di::key_code::s) app_data.m_keyDown[Im3d::Key_S] = false;
  });

  return framegraph->add_render_task<immediate_task_data>(
    "Immediate Pass",
    [=] (      immediate_task_data& data, fg::render_task_builder& builder)
    {
      data.attributes        = builder.read(retained_attributes       );
      data.cameras           = builder.read(common_data.cameras       );
      data.viewport          = builder.read(retained_viewport         );
      data.points_program    = builder.read(retained_points_program   );
      data.lines_program     = builder.read(retained_lines_program    );
      data.triangles_program = builder.read(retained_triangles_program);
      data.vertex_array      = builder.create<vertex_array_resource>("Immediate Vertex Array", vertex_array::description
      {
        { 
          {data.attributes, 4, GL_FLOAT        , false, 0, sizeof Im3d::VertexData, offsetof(Im3d::VertexData, m_positionSize)},
          {data.attributes, 4, GL_UNSIGNED_BYTE, true , 0, sizeof Im3d::VertexData, offsetof(Im3d::VertexData, m_color       )}
        }, 
        { 
          data.cameras,
          data.viewport
        }
      });
      data.target            = builder.write(target);

      data.draw_callback     = [&] (const Im3d::DrawList& draw_list)
      {
        GLenum primitive_type;
        if      (draw_list.m_primType == Im3d::DrawPrimitive_Points) primitive_type = GL_POINTS   ;
        else if (draw_list.m_primType == Im3d::DrawPrimitive_Lines ) primitive_type = GL_LINES    ;
        else                                                         primitive_type = GL_TRIANGLES;
        
        if(!camera_tag.empty())
        {
          auto cameras = framegraph->scene_cache()->entities<metadata, projection>();
          for (std::uint32_t i = 0; i < cameras.size(); ++i)
            if (cameras[i]->component<metadata>()->contains_tag(camera_tag))
              data.cameras->actual()->set_sub_data(sizeof glm::uint, sizeof glm::uint, &i);
        }

        glm::vec2 size {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()};

        data.attributes->actual()->set_sub_data(0, draw_list.m_vertexCount * sizeof Im3d::VertexData, draw_list.m_vertexData);
        data.viewport  ->actual()->set_sub_data(0, sizeof glm::vec2, &size[0]);

        data.target      ->actual()->bind();
        data.vertex_array->actual()->bind();
        if      (primitive_type == GL_POINTS) data.points_program   ->actual()->use  ();
        else if (primitive_type == GL_LINES ) data.lines_program    ->actual()->use  ();
        else                                  data.triangles_program->actual()->use  ();
        
        glClipControl             (GL_LOWER_LEFT, GL_ZERO_TO_ONE);
        gl::set_depth_test_enabled(true);
        gl::set_blending_enabled  (true);
        gl::set_point_size_enabled(true);
        gl::set_depth_function    (GL_LESS);
        gl::set_blend_equation    (GL_FUNC_ADD , GL_FUNC_ADD );
        gl::set_blend_function    (GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gl::set_viewport          ({0, 0}, {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()});
        gl::draw_arrays           (primitive_type, 0, draw_list.m_vertexCount);

        if      (primitive_type == GL_POINTS) data.points_program   ->actual()->unuse();
        else if (primitive_type == GL_LINES ) data.lines_program    ->actual()->unuse();
        else                                  data.triangles_program->actual()->unuse();
        data.vertex_array->actual()->unbind();
        data.target      ->actual()->unbind();
      };

      auto& app_data = Im3d::GetAppData();
      app_data.m_appData     = &data.draw_callback;
      app_data.drawCallback  = [ ] (const Im3d::DrawList& draw_list)
      {
        (*reinterpret_cast<std::function<void(const Im3d::DrawList&)>*>(Im3d::GetAppData().m_appData))(draw_list);
      };
    },
    [=] (const immediate_task_data& data)
    {
      auto  cameras = framegraph->scene_cache()->entities<metadata, projection>();
      auto& camera  = cameras[0];
      if (!camera_tag.empty())
        for (auto i = 0; i < cameras.size(); ++i)
          if (cameras[i]->component<metadata>()->contains_tag(camera_tag))
            camera = cameras[i];
      
      auto camera_transform  = camera->component<transform> ();
      auto camera_projection = camera->component<projection>();
      auto camera_position   = camera_transform ->translation(true);
      auto camera_direction  = camera_transform ->forward    (true);
      auto transform_matrix  = camera_transform ->matrix     (true);
      auto projection_matrix = camera_projection->matrix     ();
      
      Im3d::Mat4 native_transform_matrix(
        transform_matrix [0][0], transform_matrix [0][1], transform_matrix [0][2], transform_matrix [0][3],
        transform_matrix [1][0], transform_matrix [1][1], transform_matrix [1][2], transform_matrix [1][3],
        transform_matrix [2][0], transform_matrix [2][1], transform_matrix [2][2], transform_matrix [2][3],
        transform_matrix [3][0], transform_matrix [3][1], transform_matrix [3][2], transform_matrix [3][3]);
      Im3d::Mat4 native_projection_matrix(
        projection_matrix[0][0], projection_matrix[0][1], projection_matrix[0][2], projection_matrix[0][3],
        projection_matrix[1][0], projection_matrix[1][1], projection_matrix[1][2], projection_matrix[1][3],
        projection_matrix[2][0], projection_matrix[2][1], projection_matrix[2][2], projection_matrix[2][3],
        projection_matrix[3][0], projection_matrix[3][1], projection_matrix[3][2], projection_matrix[3][3]);

      auto& app_data = Im3d::GetAppData();
      app_data.m_deltaTime       = std::chrono::duration_cast<std::chrono::duration<float>>(framegraph->delta_cache()).count();
      app_data.m_viewportSize    = Im3d::Vec2(static_cast<float>(data.target->actual()->color_texture()->width()), static_cast<float>(data.target->actual()->color_texture()->height()));
      app_data.m_viewOrigin      = Im3d::Vec3(camera_position .x, camera_position .y, camera_position .z);
      app_data.m_viewDirection   = Im3d::Vec3(camera_direction.x, camera_direction.y, camera_direction.z);
      app_data.m_worldUp         = Im3d::Vec3(0.0f, 1.0f, 0.0f);
      app_data.m_projOrtho       = camera_projection->mode() == projection::mode::orthographic;
      app_data.m_projScaleY      = 2.0f / projection_matrix[1][1];
      app_data.m_snapTranslation = 0.0f;
      app_data.m_snapRotation    = 0.0f;
      app_data.m_snapScale       = 0.0f;

      Im3d::Vec2 cursor(static_cast<float>(di::mouse::relative_position()[0]), static_cast<float>(di::mouse::relative_position()[1]));
      cursor   = cursor / app_data.m_viewportSize * Im3d::Vec2(2.0f) - Im3d::Vec2(1.0f);
      cursor.y = -cursor.y;
      
      Im3d::Vec3 ray_origin, ray_direction;
      if (app_data.m_projOrtho)
      {
        ray_origin.x    = cursor.x / native_projection_matrix(0, 0);
        ray_origin.y    = cursor.y / native_projection_matrix(1, 1);
        ray_origin.z    = 0.0f;
        ray_origin      = native_transform_matrix * Im3d::Vec4(ray_origin, 1.0f);
        ray_direction   = native_transform_matrix * Im3d::Vec4(0.0f, 0.0f, -1.0f, 0.0f);
      }
      else 
      {
        ray_origin      = app_data.m_viewOrigin;
        ray_direction.x = cursor.x / native_projection_matrix(0, 0);
        ray_direction.y = cursor.y / native_projection_matrix(1, 1);
        ray_direction.z = -1.0f;
        ray_direction   = native_transform_matrix * Im3d::Vec4(Im3d::Normalize(ray_direction), 0.0f);
      }
      app_data.m_cursorRayOrigin    = ray_origin   ;
      app_data.m_cursorRayDirection = ray_direction;

      app_data.setCullFrustum(native_projection_matrix, false);
    
      Im3d::Draw    ();
      Im3d::NewFrame();

      gl::print_error("Error in Immediate Pass");
    });
}
}
}