#include "EngineGUI.h"
#include "Window.h"

void EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Init(window->getInternal());
  setupDarkGUIStyle();
}

void EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window, sf::Time deltaTime) {
  ImGui::SFML::Update(window->getInternal(), deltaTime);
  renderMenuBar();
  renderControlPanel();

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
