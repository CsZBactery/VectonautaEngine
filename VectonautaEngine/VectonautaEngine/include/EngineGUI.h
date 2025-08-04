#pragma once

#include "Prerequisites.h"
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

namespace EngineUtilities { template<typename T> class TSharedPointer; }
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

  void setupGreyGUIStyle();
  void setupDarkGUIStyle();
  void setupVectonautaEngineStyle();

  void processEvent(const EngineUtilities::TSharedPointer<Window>& window, const sf::Event& event);

  void setRacers(const std::vector<EngineUtilities::TSharedPointer<A_Racer>>& racers);

  bool shouldQuit() const { return m_requestQuit; }
  bool isPaused() const { return m_paused; }
  bool shouldResetWaypoints();
  float getSpeedMultiplier() const { return m_speedMultiplier; }

  Theme getCurrentTheme() const { return m_currentTheme; }
  void setTheme(Theme theme);
  void togglePause() { m_paused = !m_paused; }

private:
  void renderMenuBar();
  void renderControlPanel();
  void renderRacersPanel();

  bool m_requestQuit = false;
  bool m_paused = false;
  bool m_requestReset = false;
  float m_speedMultiplier = 1.0f;

  Theme m_currentTheme = Theme::VectonautaEngine;

  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;
};
