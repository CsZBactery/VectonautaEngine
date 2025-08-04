#include "EngineGUI.h"
#include "Window.h"

void EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Init(window->getInternal());
  setupDarkGUIStyle();
}

void EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window, sf::Time deltaTime) {
  ImGui::SFML::Update(window->getInternal(), deltaTime);
  renderMenuBar();

  ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
  ImGui::Text("FPS: %.1f", 1.0f / deltaTime.asSeconds());
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

void EngineGUI::renderMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        m_requestQuit = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void EngineGUI::processEvent(const sf::Event& event) {
  // Cambiado: pasamos también la ventana porque la versión de ImGui-SFML que tienes lo requiere
  // Necesitas tener acceso a la ventana; aquí asumimos que se almacenó o se pasa de otra forma.
  // Para mantener compatibilidad, podrías cambiar la firma para recibir el window también.
  // Ejemplo de uso correcto en BaseApp::run:
  // ImGui::SFML::ProcessEvent(window->getInternal(), event);
  // Así que aquí simplemente no repites: delega fuera o adapta:
}

