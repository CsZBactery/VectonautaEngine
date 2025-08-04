#include "EngineGUI.h"
#include "Window.h"

void EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Init(window->getInternal());
  setupDarkGUIStyle();
  setupVectonautaEngineStyle(); // estilo por defecto
}

void EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window, sf::Time deltaTime) {
  ImGui::SFML::Update(window->getInternal(), deltaTime);
  renderMenuBar();
  renderControlPanel();

  // Panel de estadísticas
  ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
  ImGui::Text("FPS: %.1f", 1.0f / deltaTime.asSeconds());
  ImGui::Text("Paused: %s", m_paused ? "Yes" : "No");
  ImGui::Text("Speed x%.2f", m_speedMultiplier);
  ImGui::End();
}

void EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Render(window->getInternal());
}

void EngineGUI::destroy() {
  ImGui::SFML::Shutdown();
}

void EngineGUI::setupGreyGUIStyle() {
  ImGui::StyleColorsClassic();
}

void EngineGUI::setupDarkGUIStyle() {
  ImGui::StyleColorsDark();
}

void EngineGUI::setupVectonautaEngineStyle() {
  ImGuiStyle& style = ImGui::GetStyle();

  style.WindowRounding = 4.0f;
  style.FrameRounding = 4.0f;
  style.GrabRounding = 4.0f;
  style.ScrollbarRounding = 4.0f;
  style.TabRounding = 2.0f;
  style.WindowBorderSize = 1.0f;
  style.FrameBorderSize = 1.0f;
  style.WindowPadding = ImVec2(10, 10);
  style.FramePadding = ImVec2(8, 4);
  style.ItemSpacing = ImVec2(8, 6);
  style.ItemInnerSpacing = ImVec2(6, 4);

  ImVec4* colors = style.Colors;
  colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.48f, 0.48f, 0.68f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.08f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.12f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.13f, 1.00f);
  colors[ImGuiCol_Border] = ImVec4(0.46f, 0.19f, 0.82f, 0.90f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.17f, 0.00f, 0.20f, 0.60f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.10f, 0.18f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.53f, 0.29f, 0.90f, 0.55f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.10f, 0.18f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.46f, 0.19f, 0.82f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.09f, 0.09f, 0.13f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.10f, 0.18f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.10f, 0.17f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.38f, 0.16f, 0.66f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.53f, 0.29f, 0.90f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.28f, 1.00f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.85f, 0.28f, 1.00f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.22f, 0.11f, 0.34f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.85f, 0.28f, 1.00f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.25f, 0.14f, 0.38f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.16f, 0.66f, 0.90f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.62f, 0.22f, 0.95f, 0.40f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.85f, 0.28f, 1.00f, 0.80f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.10f, 0.18f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
  colors[ImGuiCol_TabActive] = ImVec4(0.85f, 0.28f, 1.00f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.09f, 0.09f, 0.13f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.25f, 0.14f, 0.38f, 1.00f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.68f, 0.47f, 0.99f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.38f, 0.16f, 0.66f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.85f, 0.28f, 1.00f, 0.25f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.85f, 0.28f, 1.00f, 0.70f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.62f, 0.22f, 0.95f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.40f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.60f);
}

void EngineGUI::processEvent(const EngineUtilities::TSharedPointer<Window>& window, const sf::Event& event) {
  ImGui::SFML::ProcessEvent(window->getInternal(), event);
}

bool EngineGUI::shouldResetWaypoints() {
  if (m_requestReset) {
    m_requestReset = false;
    return true;
  }
  return false;
}

void EngineGUI::renderMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        m_requestQuit = true;
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Game")) {
      if (ImGui::MenuItem(m_paused ? "Resume" : "Pause")) {
        m_paused = !m_paused;
      }
      if (ImGui::MenuItem("Reset Waypoints")) {
        m_requestReset = true;
      }
      ImGui::Separator();
      ImGui::Text("Speed:");
      ImGui::SliderFloat("##speed", &m_speedMultiplier, 0.1f, 3.0f, "%.2f");
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void EngineGUI::renderControlPanel() {
  ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  // Tema dinámico
  ImGui::Text("Theme:");
  static int currentTheme = 2; // 0=Grey,1=Dark,2=Vectonauta
  const char* names[] = { "Grey", "Dark", "Vectonauta" };
  if (ImGui::Combo("##theme", &currentTheme, names, IM_ARRAYSIZE(names))) {
    switch (currentTheme) {
    case 0: setupGreyGUIStyle(); break;
    case 1: setupDarkGUIStyle(); break;
    case 2: setupVectonautaEngineStyle(); break;
    }
  }
  if (ImGui::Button(m_paused ? "Resume" : "Pause")) {
    m_paused = !m_paused;
  }
  if (ImGui::Button("Reset Waypoints")) {
    m_requestReset = true;
  }
  ImGui::SliderFloat("Speed Multiplier", &m_speedMultiplier, 0.1f, 3.0f, "%.2f");
  if (ImGui::Button("Exit")) {
    m_requestQuit = true;
  }
  ImGui::End();
}
