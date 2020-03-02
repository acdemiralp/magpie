#include <magpie/graphics/imgui_context.hpp>

namespace mp
{
ImGuiContext* imgui_context    ()
{
  return ImGui::GetCurrentContext();
}

void          imgui_begin_scene()
{
  auto& io = ImGui::GetIO();
  ImGui::PushStyleVar  (ImGuiStyleVar_WindowBorderSize,   0.0f);
  ImGui::PushStyleVar  (ImGuiStyleVar_WindowPadding   , { 0.0f, 0.0f });
  ImGui::PushStyleColor(ImGuiCol_WindowBg             , { 0.0f, 0.0f, 0.0f, 0.0f });
  ImGui::Begin         ("Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
  ImGui::SetWindowPos  (ImVec2(0, 0)                              , ImGuiCond_Always);
  ImGui::SetWindowSize (ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

}
void          imgui_end_scene  ()
{
  ImGui::GetWindowDrawList()->PushClipRectFullScreen();
  ImGui::End          ();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar  (2);

}
}
