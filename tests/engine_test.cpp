#include <catch.hpp>

#include <acd/random_number_generator.hpp>
#include <magpie/api.hpp>
#include <magpie/graphics/backend/opengl/render_tasks.hpp>

TEST_CASE("Engine test.", "[mp::engine]")
{
  auto engine = mp::make_default_engine();
  auto window = engine->system<mp::display_system>()->create_opengl_window("Test", di::opengl_context_settings{di::opengl_profile::core, 4, 5});
  window->set_position (std::array<std::size_t, 2>{32 , 32 });
  window->set_size     (std::array<std::size_t, 2>{800, 600});
  window->set_mode     (di::window_mode::windowed);
  window->set_swap_mode(di::opengl_swap_mode::vertical_sync);
  mp::opengl::make_default_framegraph(engine.get(), window);

  auto entity    = engine->scene()->add_entity();
  auto behaviors = entity->add_component<mp::behaviors>();
  behaviors->push_back(std::make_shared<mp::behavior>(
  [&] (                                    mp::scene* scene, mp::entity* entity)
  {
    scene->entities<mp::projection>()[0]->component<mp::projection>()->set_perspective(60.0f, float(window->size()[0]) / float(window->size()[1]), { 0.3f, 1000.0f });
  },
  [ ] (mp::frame_timer::duration duration, mp::scene* scene, mp::entity* entity)
  {
    acd::random_number_generator<std::uniform_real_distribution<float>> rng(-1.0f, 1.0f);

    ImGui::SetCurrentContext(mp::imgui_context());
    ImGui::Begin            ("FPS");
    ImGui::Text             ("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End              ();

    Im3d::SetContext        (mp::im3d_context());
    Im3d::PushDrawState     ();
    Im3d::SetSize           (5.0f);
    Im3d::BeginPoints       ();
    
    for (auto i = 0; i < 100; ++i)
    {
      Im3d::Vertex          (-2.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Vec4(rng.generate(), rng.generate(), rng.generate(), 1.0f));
    }
    
    Im3d::End               ();
    Im3d::BeginLineLoop     ();
    
    for (auto i = 0; i < 100; ++i)
    {
      Im3d::Vertex          (rng.generate(), rng.generate(), rng.generate(), Im3d::Vec4(rng.generate(), rng.generate(), rng.generate(), 1.0f));
      Im3d::Vertex          (rng.generate(), rng.generate(), rng.generate(), Im3d::Vec4(rng.generate(), rng.generate(), rng.generate(), 1.0f));
    }

    Im3d::End               ();
    Im3d::BeginTriangles    ();
    
    for (auto i = 0; i < 100; ++i)
    {
      Im3d::Vertex          (2.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Vec4(rng.generate(), rng.generate(), rng.generate(), 1.0f));
      Im3d::Vertex          (2.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Vec4(rng.generate(), rng.generate(), rng.generate(), 1.0f));
      Im3d::Vertex          (2.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Vec4(rng.generate(), rng.generate(), rng.generate(), 1.0f));
    }
    
    Im3d::End               ();
    Im3d::PopDrawState      ();
  }));

  engine->run();
}