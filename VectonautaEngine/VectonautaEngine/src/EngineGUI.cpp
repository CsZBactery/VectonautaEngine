#include "EngineGUI.h"
#include "Window.h"
#include "A_Racer.h"
#include "ECS/Transform.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <algorithm>
#include <string>
#include <cmath>

void EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Init(window->getInternal());
  setTheme(m_currentTheme);
}

void EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window, sf::Time deltaTime) {
  ImGui::SFML::Update(window->getInternal(), deltaTime);

  renderMenuBar();
  renderControlPanel();

  // Stats
  ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
  ImGui::Text("Paused: %s", m_paused ? "Yes" : "No");
  ImGui::Text("Speed x%.2f", m_speedMultiplier);
  ImGui::End();

  // Podio / racers
  ImGui::Begin("Racers / Podio", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> sorted = m_racers;
  std::sort(sorted.begin(), sorted.end(), [](const EngineUtilities::TSharedPointer<A_Racer>& a, const EngineUtilities::TSharedPointer<A_Racer>& b) {
    if (!a || !b) return false;
    return a->getProgress() > b->getProgress();
    });

  int idx = 1;
  for (auto& r : sorted) {
    if (!r) continue;
    std::string label = std::to_string(idx) + ". " + r->getName() + " (P" + std::to_string(r->getPlace() == 0 ? idx : r->getPlace()) + ")";
    float prog = r->getProgress() * 100.f;
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.1f%%", prog);
    ImGui::TextWrapped("%s %s", label.c_str(), buf);

    if (ImGui::SmallButton(("Reset##" + std::to_string(idx)).c_str())) {
      r->reset();
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      // determinar índice real en m_racers
      for (size_t i = 0; i < m_racers.size(); ++i) {
        if (m_racers[i].get() == r.get()) {
          m_selectedRacerIndex = static_cast<int>(i);
          break;
        }
      }
    }

    idx++;
  }
  ImGui::End();

  // Inspector del racer seleccionado
  if (m_selectedRacerIndex < 0 && !m_racers.empty()) m_selectedRacerIndex = 0;
  if (m_selectedRacerIndex >= 0 && m_selectedRacerIndex < static_cast<int>(m_racers.size())) {
    auto& r = m_racers[m_selectedRacerIndex];
    if (r) {
      ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("Name: %s", r->getName().c_str());
      ImGui::Text("Player ID: %d", r->getPlayerId());
      ImGui::Text("Place: %d", r->getPlace());
      ImGui::Text("Progress: %.1f%%", r->getProgress() * 100.f);

      if (auto xf = r->getComponent<Transform>()) {
        sf::Vector2f pos = xf->getPosition();
        float rot = xf->getRotation();
        sf::Vector2f scale = xf->getScale();

        ImGui::Separator();
        ImGui::Text("Transform");
        ImGui::DragFloat2("Position", &pos.x, 1.0f);
        ImGui::DragFloat("Rotation", &rot, 1.0f);
        ImGui::DragFloat2("Scale", &scale.x, 0.01f);

        xf->setPosition(pos);
        xf->setRotation(rot);
        xf->setScale(scale);
      }

      if (ImGui::Button("Select Next")) {
        m_selectedRacerIndex = (m_selectedRacerIndex + 1) % static_cast<int>(m_racers.size());
      }

      ImGui::End();
    }
  }
}

void EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Render(window->getInternal());
}

void EngineGUI::destroy() {
  ImGui::SFML::Shutdown();
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
  case Theme::Grey: setupGreyGUIStyle(); break;
  case Theme::Dark: setupDarkGUIStyle(); break;
  case Theme::VectonautaEngine: setupVectonautaEngineStyle(); break;
  }
}

void EngineGUI::processEvent(const EngineUtilities::TSharedPointer<Window>& window, const sf::Event& event) {
  ImGui::SFML::ProcessEvent(window->getInternal(), event);
}

void EngineGUI::renderMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) m_requestQuit = true;
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Game")) {
      if (ImGui::MenuItem(m_paused ? "Resume" : "Pause")) m_paused = !m_paused;
      if (ImGui::MenuItem("Reset Waypoints")) m_requestReset = true;
      ImGui::Separator();
      ImGui::Text("Speed:"); ImGui::SameLine();
      ImGui::SliderFloat("##speed_menu", &m_speedMultiplier, 0.1f, 3.0f, "%.2f");
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Theme")) {
      const char* names[] = { "Grey", "Dark", "VectonautaEngine" };
      for (int i = 0; i < 3; ++i) {
        bool selected = (static_cast<int>(m_currentTheme) == i);
        if (ImGui::MenuItem(names[i], nullptr, selected)) {
          setTheme(static_cast<Theme>(i));
        }
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void EngineGUI::renderControlPanel() {
  ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  const char* themeNames[] = { "Grey", "Dark", "VectonautaEngine" };
  int current = static_cast<int>(m_currentTheme);
  if (ImGui::Combo("Theme:", &current, themeNames, IM_ARRAYSIZE(themeNames))) {
    setTheme(static_cast<Theme>(current));
  }

  if (ImGui::Button(m_paused ? "Resume" : "Pause")) m_paused = !m_paused;
  ImGui::SameLine();
  if (ImGui::Button("Reset All")) m_requestReset = true;

  ImGui::SliderFloat("Speed Multiplier", &m_speedMultiplier, 0.1f, 3.0f, "%.2f");

  if (ImGui::Button("Exit")) m_requestQuit = true;
  ImGui::End();
}

void EngineGUI::setupGreyGUIStyle() {
  ImGui::StyleColorsClassic();
}

void EngineGUI::setupDarkGUIStyle() {
  ImGui::StyleColorsDark();
}

void EngineGUI::setupVectonautaEngineStyle() {
  ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 6.f;
  style.FrameRounding = 4.f;
  style.GrabRounding = 4.f;
  style.ItemSpacing = ImVec2(8, 6);
  style.ItemInnerSpacing = ImVec2(6, 4);
  style.WindowPadding = ImVec2(10, 10);
  style.FramePadding = ImVec2(8, 5);

  ImVec4 gold = ImVec4(0.92f, 0.75f, 0.20f, 1.0f);
  ImVec4* colors = style.Colors;
  colors[ImGuiCol_Button] = ImVec4(0.18f, 0.10f, 0.28f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = gold;
  colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.65f, 0.18f, 1.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.08f, 0.15f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.15f, 0.40f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.20f, 0.55f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.06f, 0.12f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.35f, 0.20f, 0.55f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.10f, 0.18f, 1.00f);
  colors[ImGuiCol_Text] = ImVec4(0.96f, 0.94f, 0.88f, 1.00f);
  colors[ImGuiCol_Border] = gold;
  colors[ImGuiCol_Separator] = gold;
  colors[ImGuiCol_TabActive] = gold;
}

void EngineGUI::setRacers(const std::vector<EngineUtilities::TSharedPointer<A_Racer>>& racers) {
  m_racers = racers;
  if (m_selectedRacerIndex >= static_cast<int>(m_racers.size())) {
    m_selectedRacerIndex = static_cast<int>(m_racers.size()) - 1;
  }
}
