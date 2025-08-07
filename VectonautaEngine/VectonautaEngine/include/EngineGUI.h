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
  enum class Theme { Grey = 0, Dark = 1, VectonautaEngine = 2 };

  EngineGUI() = default;
  ~EngineGUI() = default;

  void init(const EngineUtilities::TSharedPointer<Window>& window);
  void update(const EngineUtilities::TSharedPointer<Window>& window,
    sf::Time deltaTime,
    float raceTimer);
  void render(const EngineUtilities::TSharedPointer<Window>& window);
  void destroy();

  void processEvent(const EngineUtilities::TSharedPointer<Window>& window,
    const sf::Event& event);

  bool shouldQuit() const { return m_requestQuit; }
  bool isPaused() const { return m_paused; }
  bool shouldResetWaypoints()
  {
    if (m_requestReset) { m_requestReset = false; return true; }
    return false;
  }
  float getSpeedMultiplier() const { return m_speedMultiplier; }

  void setRacers(const std::vector<EngineUtilities::TSharedPointer<A_Racer>>& racers)
  {
    m_racers = racers;
  }

  void setTheme(Theme theme);

private:
  void renderMenuBar();
  void renderControlPanel();

  void setupGreyGUIStyle();
  void setupDarkGUIStyle();
  void setupVectonautaEngineStyle();

  bool m_requestQuit = false;
  bool m_requestReset = false;
  bool m_paused = false;
  float m_speedMultiplier = 1.f;
  Theme m_currentTheme = Theme::VectonautaEngine;
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;
};
