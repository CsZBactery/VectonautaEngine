#pragma once

#include "Prerequisites.h"
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

class Window;

class EngineGUI {
public:
  EngineGUI() = default;
  ~EngineGUI() = default;

  void init(const EngineUtilities::TSharedPointer<Window>& window);
  void update(const EngineUtilities::TSharedPointer<Window>& window, sf::Time deltaTime);
  void render(const EngineUtilities::TSharedPointer<Window>& window);
  void destroy();

  void setupGreyGUIStyle();
  void setupDarkGUIStyle();

  // Ahora recibe ventana también
  void processEvent(const EngineUtilities::TSharedPointer<Window>& window, const sf::Event& event);

  // Flags / estados expuestos
  bool shouldQuit() const { return m_requestQuit; }
  bool isPaused() const { return m_paused; }
  bool shouldResetWaypoints(); // consume el flag y lo resetea
  float getSpeedMultiplier() const { return m_speedMultiplier; }

  // Auxiliares
  void togglePause() { m_paused = !m_paused; }

private:
  void renderMenuBar();
  void renderControlPanel(); // panel flotante

  bool m_requestQuit = false;
  bool m_paused = false;
  bool m_requestReset = false;
  float m_speedMultiplier = 1.0f; // multiplicador de velocidad
};
