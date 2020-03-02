## Magpie - Minimalist Visualization Engine ##
Approximately 15 minutes to setup, including downloading and installing the dependencies.

### Motivation ###
I needed a C++ library/engine to in-situ visualize numeric compute as points/lines/triangles, and configure it using a basic UI. 
Unity3D and Unreal are not particularly efficient at immediate rendering, and are hard to couple with external dependencies. 
ParaView/Catalyst stack requires a lot of configuration, and is too heavyweight for the purpose. 
Hence, I decided to recycle parts of a never-published game engine, leading to this project.

### Requirements ###
- A C++17 Compiler ([Clang 5+](https://clang.llvm.org/get_started.html), [GCC 7+](https://gcc.gnu.org) or [MSVC 14.1+](https://www.visualstudio.com/vs))
- [CMake 3.11+](https://cmake.org/download)
- [Conan 1.1+](https://conan.io)
- [Git](https://git-scm.com/downloads)

### Building ###
- Download and install the requirements above.
- CMake. The majority of the dependencies will be downloaded, built and linked automatically via Conan through Bintray.

### Usage ###
Three lines to create a window, and setup an OpenGL rendering system: 
```cpp
#include <magpie/api.hpp>
#include <magpie/graphics/backend/opengl/render_tasks.hpp>
int main (int argc, char** argv)
{
  auto engine = mp::make_default_engine();
  auto window = engine->system<mp::display_system>()->create_opengl_window("Example", di::opengl_context_settings{di::opengl_profile::core, 4, 5});
  mp::opengl::make_default_framegraph(engine.get(), window);
}
```
You may then create an entity with a behavior and use Im3d or ImGui commands as following:
```cpp
auto entity    = engine->scene()->add_entity();
auto behaviors = entity->add_component<mp::behaviors>();
behaviors->push_back(std::make_shared<mp::behavior>(
[&] (                                    mp::scene* scene, mp::entity* entity)
{

},
[ ] (mp::frame_timer::duration duration, mp::scene* scene, mp::entity* entity)
{
  ImGui::SetCurrentContext(mp::imgui_context());
  ImGui::Begin            ("FPS");
  ImGui::Text             ("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End              ();
}));
```
See `tests/engine_test.cpp` for further detail.