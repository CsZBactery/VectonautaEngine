#include "EngineGUI.h"
#include "Window.h"

void
EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
  // Initalize the ImGui Resource
  ImGui::SFML::Init(*window->m_windowPtr);
  setupDarkGUIStyle();
}

void
EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window,
  sf::Time deltaTime) {
  ImGui::SFML::Update(*window->m_windowPtr, deltaTime);
  barMenu();
}

void
EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Render(*window->m_windowPtr);
}
void
EngineGUI::processEvent(const sf::Window& window, const sf::Event& event) {
  ImGui::SFML::ProcessEvent(window, event);
}
void
EngineGUI::destroy() {
  ImGui::SFML::Shutdown();
}