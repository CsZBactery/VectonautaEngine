#pragma once

#include "Prerequisites.h"
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>

class Window;
class A_Racer;

class EngineGUI {
public:
  enum class Theme {
    Grey = 0,
    Dark = 1,
    VectonautaEngine = 2
  };

  EngineGUI() = default;
  ~EngineGUI() = default;

  void init(const EngineUtilities::TSharedPointer<Window>& window);
  void update(const EngineUtilities::TSharedPointer<Window>& window, sf::Time deltaTime);
  void render(const EngineUtilities::TSharedPointer<Window>& window);
  void destroy();

  // Estilos
  void setupGreyGUIStyle();
  void setupDarkGUIStyle();
  void setupVectonautaEngineStyle();

  // Eventos
  void processEvent(const EngineUtilities::TSharedPointer<Window>& window, const sf::Event& event);

  // Flags / estados
  bool shouldQuit() const { return m_requestQuit; }
  bool isPaused() const { return m_paused; }
  bool shouldResetWaypoints(); // consume el flag y lo resetea
  float getSpeedMultiplier() const { return m_speedMultiplier; }

  // Tema actual
  Theme getCurrentTheme() const { return m_currentTheme; }
  void setTheme(Theme theme);
  void togglePause() { m_paused = !m_paused; }

  // Racers (para podio / inspector)
  void setRacers(const std::vector<EngineUtilities::TSharedPointer<A_Racer>>& racers);

private:
  void renderMenuBar();
  void renderControlPanel();

  bool m_requestQuit = false;
  bool m_paused = false;
  bool m_requestReset = false;
  float m_speedMultiplier = 1.0f;
  Theme m_currentTheme = Theme::VectonautaEngine;

  // Selección para inspector
  int m_selectedRacerIndex = -1;

  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;
};
