#include "EngineGUI.h"
#include "Window.h"
#include "A_Racer.h"
#include <algorithm>

// Nombres de los temas para el combo
static const char* kThemeNames[] = { "Grey", "Dark", "VectonautaEngine" };

void EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
  // Inicializa ImGui‐SFML sobre tu sf::RenderWindow interno
  ImGui::SFML::Init(window->getInternal());
  // Aplica el tema inicial
  setTheme(m_currentTheme);
}

void EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window, sf::Time deltaTime) {
  ImGui::SFML::Update(window->getInternal(), deltaTime);
  renderMenuBar();
  renderControlPanel();
}

void EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Render(window->getInternal());
}

void EngineGUI::destroy() {
  ImGui::SFML::Shutdown();
}

void EngineGUI::processEvent(const EngineUtilities::TSharedPointer<Window>& window, const sf::Event& event) {
  ImGui::SFML::ProcessEvent(window->getInternal(), event);
}

void EngineGUI::setRacers(const std::vector<EngineUtilities::TSharedPointer<A_Racer>>& racers) {
  m_racers = racers;
}

bool EngineGUI::shouldResetWaypoints() {
  if (m_requestReset) {
    m_requestReset = false;
    return true;
  }
  return false;
}

void EngineGUI::setTheme(Theme theme) {
  m_currentTheme = theme;
  switch (theme) {
  case Theme::Grey:             setupGreyGUIStyle();          break;
  case Theme::Dark:             setupDarkGUIStyle();          break;
  case Theme::VectonautaEngine: setupVectonautaEngineStyle(); break;
  }
}

// Barra de menús superior
void EngineGUI::renderMenuBar() {
  if (!ImGui::BeginMainMenuBar()) return;
  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("Exit")) m_requestQuit = true;
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Game")) {
    if (ImGui::MenuItem(m_paused ? "Resume" : "Pause")) togglePause();
    if (ImGui::MenuItem("Reset Waypoints")) m_requestReset = true;
    ImGui::Separator();
    ImGui::Text("Speed: %.2f", m_speedMultiplier);
    ImGui::SliderFloat("##speed_menu", &m_speedMultiplier, 0.1f, 3.0f, "%.2f");
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Theme")) {
    int idx = static_cast<int>(m_currentTheme);
    if (ImGui::Combo("##theme_menu", &idx, kThemeNames, IM_ARRAYSIZE(kThemeNames))) {
      setTheme(static_cast<Theme>(idx));
    }
    ImGui::EndMenu();
  }
  ImGui::EndMainMenuBar();
}

// Panel de controles y de podio
void EngineGUI::renderControlPanel() {
  ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  // Selector de tema
  ImGui::Text("Theme:");
  int idx = static_cast<int>(m_currentTheme);
  if (ImGui::Combo("##theme", &idx, kThemeNames, IM_ARRAYSIZE(kThemeNames))) {
    setTheme(static_cast<Theme>(idx));
  }

  // Pausa / reset global
  if (ImGui::Button(m_paused ? "Resume" : "Pause")) togglePause();
  ImGui::SameLine();
  if (ImGui::Button("Reset All")) m_requestReset = true;

  // Speed
  ImGui::SliderFloat("Speed Multiplier", &m_speedMultiplier, 0.1f, 3.0f, "%.2f");

  // Exit
  if (ImGui::Button("Exit")) m_requestQuit = true;

  ImGui::End();

  // Panel de podio / racers
  ImGui::Begin("Racers / Podio", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  struct Entry { EngineUtilities::TSharedPointer<A_Racer> r; float prog; };
  std::vector<Entry> list;
  for (auto& r : m_racers) if (r) list.push_back({ r, r->getProgress() });
  std::sort(list.begin(), list.end(), [](auto& a, auto& b) { return a.prog > b.prog; });

  for (size_t i = 0; i < list.size(); ++i) {
    auto& racer = list[i].r;
    ImGui::PushID(racer->getPlayerId());
    ImGui::Text("%zu. %s (P%d)", i + 1, racer->getName().c_str(), racer->getPlayerId());
    ImGui::SameLine();
    ImGui::Text("Prog: %.1f%%", list[i].prog * 100.f);
    if (racer->getPlace() > 0) {
      ImGui::SameLine();
      ImGui::Text("Finished: %d", racer->getPlace());
    }
    if (ImGui::SmallButton("Reset")) racer->reset();
    ImGui::Separator();
    ImGui::PopID();
  }

  ImGui::End();
}

// Estilos
void EngineGUI::setupGreyGUIStyle() {
  ImGui::StyleColorsClassic();
}

void EngineGUI::setupDarkGUIStyle() {
  ImGui::StyleColorsDark();
}

void EngineGUI::setupVectonautaEngineStyle() {
  ImGuiStyle& style = ImGui::GetStyle();

  // Configuración básica
  style.WindowRounding = 4; style.FrameRounding = 4;
  style.GrabRounding = 4; style.ScrollbarRounding = 4;
  style.TabRounding = 2;
  style.WindowBorderSize = 1;
  style.FrameBorderSize = 1;
  style.WindowPadding = ImVec2(10, 10);
  style.FramePadding = ImVec2(8, 4);
  style.ItemSpacing = ImVec2(8, 6);
  style.ItemInnerSpacing = ImVec2(6, 4);

  // Paleta dorada
  ImVec4 gold = ImVec4(0.83f, 0.69f, 0.22f, 1.00f);
  ImVec4 goldDim = ImVec4(0.70f, 0.58f, 0.18f, 1.00f);
  ImVec4 goldLight = ImVec4(1.00f, 0.85f, 0.40f, 1.00f);
  ImVec4 darkBg = ImVec4(0.06f, 0.06f, 0.08f, 1.00f);
  ImVec4 panelBg = ImVec4(0.08f, 0.08f, 0.12f, 1.00f);
  ImVec4 frameBg = ImVec4(0.10f, 0.09f, 0.15f, 1.00f);

  ImVec4* c = style.Colors;
  c[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
  c[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
  c[ImGuiCol_WindowBg] = darkBg;
  c[ImGuiCol_ChildBg] = panelBg;
  c[ImGuiCol_PopupBg] = panelBg;
  c[ImGuiCol_Border] = goldDim;
  c[ImGuiCol_BorderShadow] = ImVec4(0.10f, 0.08f, 0.05f, 0.50f);
  c[ImGuiCol_FrameBg] = frameBg;
  c[ImGuiCol_FrameBgHovered] = goldDim;
  c[ImGuiCol_FrameBgActive] = gold;
  c[ImGuiCol_TitleBg] = panelBg;
  c[ImGuiCol_TitleBgActive] = gold;
  c[ImGuiCol_TitleBgCollapsed] = panelBg;
  c[ImGuiCol_MenuBarBg] = frameBg;
  c[ImGuiCol_ScrollbarBg] = panelBg;
  c[ImGuiCol_ScrollbarGrab] = goldDim;
  c[ImGuiCol_ScrollbarGrabHovered] = goldLight;
  c[ImGuiCol_ScrollbarGrabActive] = gold;
  c[ImGuiCol_CheckMark] = gold;
  c[ImGuiCol_SliderGrab] = goldDim;
  c[ImGuiCol_SliderGrabActive] = goldLight;
  c[ImGuiCol_Button] = ImVec4(0.25f, 0.16f, 0.25f, 1.00f);
  c[ImGuiCol_ButtonHovered] = goldDim;
  c[ImGuiCol_ButtonActive] = gold;
  c[ImGuiCol_Header] = ImVec4(0.22f, 0.16f, 0.30f, 1.00f);
  c[ImGuiCol_HeaderHovered] = goldDim;
  c[ImGuiCol_HeaderActive] = gold;
  c[ImGuiCol_Separator] = goldDim;
  c[ImGuiCol_SeparatorHovered] = goldLight;
  c[ImGuiCol_SeparatorActive] = gold;
  c[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.22f, 0.35f, 0.40f);
  c[ImGuiCol_ResizeGripHovered] = goldDim;
  c[ImGuiCol_ResizeGripActive] = gold;
  c[ImGuiCol_Tab] = frameBg;
  c[ImGuiCol_TabHovered] = goldDim;
  c[ImGuiCol_TabActive] = gold;
  c[ImGuiCol_TabUnfocused] = panelBg;
  c[ImGuiCol_TabUnfocusedActive] = ImVec4(0.35f, 0.25f, 0.45f, 1.00f);
  c[ImGuiCol_PlotLines] = goldLight;
  c[ImGuiCol_PlotLinesHovered] = gold;
  c[ImGuiCol_PlotHistogram] = ImVec4(0.50f, 0.35f, 0.60f, 1.00f);
  c[ImGuiCol_PlotHistogramHovered] = gold;
  c[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.85f, 0.40f, 0.25f);
  c[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.85f, 0.40f, 0.70f);
  c[ImGuiCol_NavHighlight] = gold;
  c[ImGuiCol_NavWindowingHighlight] = ImVec4(0.83f, 0.69f, 0.22f, 0.45f);
  c[ImGuiCol_NavWindowingDimBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.40f);
  c[ImGuiCol_ModalWindowDimBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.60f);
}
