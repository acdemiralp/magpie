#include <magpie/graphics/ui_system.hpp>

#include <di/systems/input/mouse.hpp>
#include <gl/all.hpp>

#include <magpie/display/display_system.hpp>
#include <magpie/input/input_system.hpp>

namespace mp
{
ui_system::ui_system (display_system* display_system, input_system* input_system) : display_system_(display_system), input_system_(input_system), context_(nullptr)
{
  if (!display_system_) throw std::runtime_error("UI system requires a valid display system!");
  if (!input_system_  ) throw std::runtime_error("UI system requires a valid input system!"  );
}

void ui_system::prepare(                                    scene* scene)
{
  if (context_) ImGui::DestroyContext(context_);
  context_ = ImGui::CreateContext();

  auto& io = ImGui::GetIO();
  io.KeyMap[ImGuiKey_Tab       ] = SDL_SCANCODE_TAB      ;
  io.KeyMap[ImGuiKey_LeftArrow ] = SDL_SCANCODE_LEFT     ;
  io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT    ;
  io.KeyMap[ImGuiKey_UpArrow   ] = SDL_SCANCODE_UP       ;
  io.KeyMap[ImGuiKey_DownArrow ] = SDL_SCANCODE_DOWN     ;
  io.KeyMap[ImGuiKey_PageUp    ] = SDL_SCANCODE_PAGEUP   ;
  io.KeyMap[ImGuiKey_PageDown  ] = SDL_SCANCODE_PAGEDOWN ;
  io.KeyMap[ImGuiKey_Home      ] = SDL_SCANCODE_HOME     ;
  io.KeyMap[ImGuiKey_End       ] = SDL_SCANCODE_END      ;
  io.KeyMap[ImGuiKey_Delete    ] = SDL_SCANCODE_DELETE   ;
  io.KeyMap[ImGuiKey_Backspace ] = SDL_SCANCODE_BACKSPACE;
  io.KeyMap[ImGuiKey_Enter     ] = SDL_SCANCODE_RETURN   ;
  io.KeyMap[ImGuiKey_Escape    ] = SDL_SCANCODE_ESCAPE   ;
  io.KeyMap[ImGuiKey_A         ] = SDL_SCANCODE_A        ;
  io.KeyMap[ImGuiKey_C         ] = SDL_SCANCODE_C        ;
  io.KeyMap[ImGuiKey_V         ] = SDL_SCANCODE_V        ;
  io.KeyMap[ImGuiKey_X         ] = SDL_SCANCODE_X        ;
  io.KeyMap[ImGuiKey_Y         ] = SDL_SCANCODE_Y        ;
  io.KeyMap[ImGuiKey_Z         ] = SDL_SCANCODE_Z        ;
  io.SetClipboardTextFn = [] (void*, const char* text) -> void        {       SDL_SetClipboardText(text);};
  io.GetClipboardTextFn = [] (void*)                   -> const char* {return SDL_GetClipboardText()    ;};
  io.ClipboardUserData  = nullptr;
  io.Fonts->TexID       = nullptr;

  for (auto cursor : cursors_)
    if (cursor) SDL_FreeCursor(cursor);
  cursors_[ImGuiMouseCursor_Arrow     ] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW   );
  cursors_[ImGuiMouseCursor_TextInput ] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM   );
  cursors_[ImGuiMouseCursor_ResizeNS  ] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS  );
  cursors_[ImGuiMouseCursor_ResizeEW  ] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE  );
  cursors_[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
  cursors_[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);

  if (display_system_->windows().size() > 0)
  {
    auto main_window_size = display_system_->windows()[0]->size();
    io.DisplaySize        = ImVec2(static_cast<float>(main_window_size[0]), static_cast<float>(main_window_size[1]));
#ifdef _WIN32
    io.ImeWindowHandle    = std::get<HWND>(display_system_->windows()[0]->driver_data());
#endif
  }

  input_system_->on_mouse_wheel  .connect([ ] (std::array<std::int32_t, 2> amount) { ImGui::GetIO().MouseWheel += amount[1];              });
  input_system_->on_mouse_press  .connect([ ] (std::size_t                 index ) { ImGui::GetIO().MouseDown[index - 1] = true ;         });
  input_system_->on_mouse_release.connect([ ] (std::size_t                 index ) { ImGui::GetIO().MouseDown[index - 1] = false;         });
  input_system_->on_text_input   .connect([ ] (std::string                 text  ) { ImGui::GetIO().AddInputCharactersUTF8(text.c_str()); });
  input_system_->on_key_press    .connect([ ] (di::key                     key   )
  {
    auto& io = ImGui::GetIO();
    io.KeysDown[static_cast<std::uint32_t>(key.scan_code)] = true;
    io.KeyShift = (key.modifier & (di::key_modifier::left_shift | di::key_modifier::right_shift)) != di::key_modifier::none;
    io.KeyCtrl  = (key.modifier & (di::key_modifier::left_ctrl  | di::key_modifier::right_ctrl )) != di::key_modifier::none;
    io.KeyAlt   = (key.modifier & (di::key_modifier::left_alt   | di::key_modifier::right_alt  )) != di::key_modifier::none;
    io.KeySuper = (key.modifier & (di::key_modifier::left_gui   | di::key_modifier::right_gui  )) != di::key_modifier::none;
  });
  input_system_->on_key_release  .connect([ ] (di::key                     key   )
  {
    auto& io = ImGui::GetIO();
    io.KeysDown[static_cast<std::uint32_t>(key.scan_code)] = false;
    io.KeyShift = (key.modifier & (di::key_modifier::left_shift | di::key_modifier::right_shift)) != di::key_modifier::none;
    io.KeyCtrl  = (key.modifier & (di::key_modifier::left_ctrl  | di::key_modifier::right_ctrl )) != di::key_modifier::none;
    io.KeyAlt   = (key.modifier & (di::key_modifier::left_alt   | di::key_modifier::right_alt  )) != di::key_modifier::none;
    io.KeySuper = (key.modifier & (di::key_modifier::left_gui   | di::key_modifier::right_gui  )) != di::key_modifier::none;
  });

  ImGui::StyleColorsDark();
  ImGui::NewFrame       ();
}
void ui_system::update (const frame_timer::duration& delta, scene* scene)
{
  auto& io             = ImGui::GetIO();
  auto  mouse_position = di::mouse::relative_position();
  io.MousePos          = ImVec2(static_cast<float>(mouse_position[0]), static_cast<float>(mouse_position[1]));
  io.DeltaTime         = std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();

  if(display_system_->windows().size() > 0)
  {
    auto main_window_size = display_system_->windows()[0]->size();
    io.DisplaySize        = ImVec2(static_cast<float>(main_window_size[0]), static_cast<float>(main_window_size[1]));
  }

  const auto cursor = ImGui::GetMouseCursor();
  if(io.MouseDrawCursor || cursor == ImGuiMouseCursor_None) di::mouse::set_visible(false);
  else
  {
    di::mouse::set_visible(true);
    SDL_SetCursor(cursors_[cursor] ? cursors_[cursor] : cursors_[ImGuiMouseCursor_Arrow]);
  }
}
}