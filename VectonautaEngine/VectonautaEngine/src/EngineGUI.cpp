#include "EngineGUI.h"
#include "Window.h"
#include "A_Racer.h"
#include <imgui-SFML.h>
#include <algorithm>

void EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window)
{
  ImGui::SFML::Init(window->getInternal());
  setTheme(m_currentTheme);
}

void EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window,
  sf::Time deltaTime,
  float raceTimer)
{
  ImGui::SFML::Update(window->getInternal(), deltaTime);

  renderMenuBar();
  renderControlPanel();

  // Panel stats
  ImGui::Begin("Stats", nullptr,
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
  ImGui::Text("Timer: %.2f s", raceTimer);
  ImGui::End();

  // Podio / Racers
  ImGui::Begin("Racers / Podio", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  // ordenar por progreso descendente
  auto sorted = m_racers;
  std::sort(sorted.begin(), sorted.end(),
    [](auto& a, auto& b) {
      return a->getProgress() > b->getProgress();
    });
  int idx = 1;
  for (auto& r : sorted) {
    std::string label = std::to_string(idx) + ". " +
      r->getName() +
      " (P" + std::to_string(r->getPlace() ? r->getPlace() : idx) + ")";
    char buf[32];
    std::snprintf(buf, 32, "%.1f%%", r->getProgress() * 100.f);
    ImGui::Text("%s %s", label.c_str(), buf);
    if (ImGui::SmallButton(("Reset##" + std::to_string(idx)).c_str()))
      r->reset();
    idx++;
  }
  ImGui::End();
}

void EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window)
{
  ImGui::SFML::Render(window->getInternal());
}

void EngineGUI::destroy()
{
  ImGui::SFML::Shutdown();
}

void EngineGUI::processEvent(const EngineUtilities::TSharedPointer<Window>& window,
  const sf::Event& event)
{
  ImGui::SFML::ProcessEvent(window->getInternal(), event);
}

void EngineGUI::setTheme(Theme theme)
{
  m_currentTheme = theme;
  switch (theme) {
  case Theme::Grey:            setupGreyGUIStyle();           break;
  case Theme::Dark:            setupDarkGUIStyle();           break;
  case Theme::VectonautaEngine: setupVectonautaEngineStyle(); break;
  }
}

void EngineGUI::renderMenuBar()
{
  if (!ImGui::BeginMainMenuBar()) return;
  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("Exit")) m_requestQuit = true;
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Game")) {
    if (ImGui::MenuItem(m_paused ? "Resume" : "Pause")) m_paused = !m_paused;
    if (ImGui::MenuItem("Reset")) m_requestReset = true;
    ImGui::Separator();
    ImGui::Text("Speed:");
    ImGui::SliderFloat("##speed", &m_speedMultiplier, 0.1f, 3.0f, "%.2f");
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Theme")) {
    const char* names[] = { "Grey","Dark","VectonautaEngine" };
    for (int i = 0;i < 3;i++)
      if (ImGui::MenuItem(names[i], nullptr, m_currentTheme == Theme(i)))
        setTheme(Theme(i));
    ImGui::EndMenu();
  }
  ImGui::EndMainMenuBar();
}

void EngineGUI::renderControlPanel()
{
  ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  const char* names[] = { "Grey","Dark","VectonautaEngine" };
  int cur = int(m_currentTheme);
  if (ImGui::Combo("Theme", &cur, names, 3))
    setTheme(Theme(cur));

  if (ImGui::Button(m_paused ? "Resume" : "Pause")) m_paused = !m_paused;
  ImGui::SameLine();
  if (ImGui::Button("Reset All")) m_requestReset = true;

  ImGui::SliderFloat("Speed Mul", &m_speedMultiplier, 0.1f, 3.f, "%.2f");
  if (ImGui::Button("Exit")) m_requestQuit = true;
  ImGui::End();
}

void EngineGUI::setupGreyGUIStyle() { ImGui::StyleColorsClassic(); }
void EngineGUI::setupDarkGUIStyle() { ImGui::StyleColorsDark(); }
void EngineGUI::setupVectonautaEngineStyle()
{
  ImGui::StyleColorsDark();
  auto& s = ImGui::GetStyle();
  s.WindowRounding = 6.f;  s.FrameRounding = 4.f;
  ImVec4 gold{ 0.92f,0.75f,0.20f,1.0f };
  s.Colors[ImGuiCol_ButtonHovered] = gold;
  s.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.65f, 0.18f, 1.0f);
  s.Colors[ImGuiCol_Border] = gold;
  // … puedes ajustar más colores …
}
