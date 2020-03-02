#include <magpie/graphics/backend/opengl/render_tasks/ui_render_task.hpp>

#include <gl/auxiliary/glm_uniforms.hpp>
#include <gl/all.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include <magpie/core/configuration.hpp>
#include <magpie/graphics/backend/glsl/ui_fragment_shader.hpp>
#include <magpie/graphics/backend/glsl/ui_vertex_shader.hpp>

namespace mp
{
namespace opengl
{
fg::render_task<ui_task_data>* add_ui_render_task (renderer* framegraph, framebuffer_resource* target)
{
  const auto retained_attributes = framegraph->add_retained_resource<buffer_description, gl::buffer>                      ("UI Vertices"  , buffer_description{configuration::instance().get<GLsizeiptr>("rendering.ui_render_task.vertex_buffer_size", 4e+6)});
  const auto retained_indices    = framegraph->add_retained_resource<buffer_description, gl::buffer>                      ("UI Indices"   , buffer_description{configuration::instance().get<GLsizeiptr>("rendering.ui_render_task.index_buffer_size" , 4e+6)});
  const auto retained_projection = framegraph->add_retained_resource<buffer_description, gl::buffer>                      ("UI Projection", buffer_description{sizeof(glm::mat4)});
  const auto retained_program    = framegraph->add_retained_resource<graphics_program_resource::description_type, program>("UI Program"   , program::graphics_description
  {
    glsl::ui_vertex_shader, 
    glsl::ui_fragment_shader
  });
  const auto retained_vertex_array = framegraph->add_retained_resource<vertex_array::description, vertex_array>             ("UI Vertex Array", vertex_array::description
  {
    { 
      {retained_attributes, 2, GL_FLOAT},
      {retained_attributes, 2, GL_FLOAT},
      {retained_attributes, 4, GL_UNSIGNED_BYTE}
    }, 
    {
      retained_projection
    }, 
    retained_indices
  });

  std::uint8_t* pixels;
  std::int32_t  width, height;
  ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  const auto retained_texture = framegraph->add_retained_resource<texture_description, gl::texture_2d>("UI Texture", texture_description {{static_cast<int>(width), static_cast<int>(height), 1}, GL_RGBA8});
  retained_texture->actual()->set_sub_image(0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  ImGui::GetIO().Fonts->TexID = reinterpret_cast<void*>(static_cast<std::size_t>(retained_texture->actual()->id()));

  return framegraph->add_render_task<ui_task_data>(
    "UI Pass",
    [&] (      ui_task_data& data, fg::render_task_builder& builder)
    {
      data.attributes   = builder.read (retained_attributes  );
      data.indices      = builder.read (retained_indices     );
      data.projection   = builder.read (retained_projection  );
      data.texture      = builder.read (retained_texture     );
      data.program      = builder.read (retained_program     );
      data.vertex_array = builder.read (retained_vertex_array);
      data.target       = builder.write(target);
    },
    [=] (const ui_task_data& data)
    {
      auto& io = ImGui::GetIO();

      auto program       = data.program     ->actual();
      auto vertex_array  = data.vertex_array->actual();
      auto target        = data.target      ->actual();

      program     ->use   ();
      vertex_array->bind  ();
      target      ->bind  ();

      gl::texture_handle handle(*data.texture->actual());
      if (!handle.is_resident()) handle.set_resident(true);
      program->set_uniform_handle(0, handle);

      const auto projection = glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 0.0f, 1.0f);
      data.projection->actual()->set_sub_data(0, sizeof glm::mat4, &projection);
    
      vertex_array->set_vertex_buffer   (0, *data.attributes->actual(), 0, sizeof ImDrawVert);
      vertex_array->set_vertex_buffer   (1, *data.attributes->actual(), 0, sizeof ImDrawVert);
      vertex_array->set_vertex_buffer   (2, *data.attributes->actual(), 0, sizeof ImDrawVert);
      vertex_array->set_attribute_format(0, 2, GL_FLOAT        , false, IM_OFFSETOF(ImDrawVert, pos));
      vertex_array->set_attribute_format(1, 2, GL_FLOAT        , false, IM_OFFSETOF(ImDrawVert, uv ));
      vertex_array->set_attribute_format(2, 4, GL_UNSIGNED_BYTE, true , IM_OFFSETOF(ImDrawVert, col));

      gl::set_blending_enabled            (true );
      gl::set_blend_function              (GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      gl::set_polygon_face_culling_enabled(false);
      gl::set_depth_test_enabled          (false);
      gl::set_scissor_test_enabled        (true );
      gl::set_viewport                    ({0, 0}, {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()});
      
      ImGui::Render();
      const auto draw_data = ImGui::GetDrawData();
      for(auto i = 0; i < draw_data->CmdListsCount; ++i)
      {
        const ImDrawList* command_list = draw_data->CmdLists[i];
        const ImDrawIdx*  index_offset = nullptr;
        data.attributes->actual()->set_sub_data(0, static_cast<GLsizeiptr>(command_list->VtxBuffer.Size) * sizeof(ImDrawVert), static_cast<const GLvoid*>(command_list->VtxBuffer.Data));
        data.indices   ->actual()->set_sub_data(0, static_cast<GLsizeiptr>(command_list->IdxBuffer.Size) * sizeof(ImDrawIdx) , static_cast<const GLvoid*>(command_list->IdxBuffer.Data));
        for(auto j = 0; j < command_list->CmdBuffer.Size; ++j)
        {
          auto& command = command_list->CmdBuffer[j];
          gl::set_scissor(
            {static_cast<int>(command.ClipRect.x)                     , static_cast<int>(io.DisplaySize.y   - command.ClipRect.w)},
            {static_cast<int>(command.ClipRect.z - command.ClipRect.x), static_cast<int>(command.ClipRect.w - command.ClipRect.y)});
          gl::draw_elements(GL_TRIANGLES, static_cast<GLsizei>(command.ElemCount), GL_UNSIGNED_SHORT, index_offset);
          index_offset += command.ElemCount;
        }
      }
      ImGui::NewFrame();

      gl::set_scissor_test_enabled(false);

      target      ->unbind();
      vertex_array->unbind();
      program     ->unuse ();

      gl::print_error("Error in UI Pass");
    });
}
}
}