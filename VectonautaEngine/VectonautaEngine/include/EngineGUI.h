#pragma once

/**
 * @file EngineGUI.h
 * @brief Declares the EngineGUI class: ImGui-based HUD/menus for the engine (pause, speed, theme, racers).
 */

#include "Prerequisites.h"
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>

class Window;
class A_Racer;

/**
 * @class EngineGUI
 * @brief Immediate-mode GUI wrapper using ImGui-SFML: menús, panel de control, stats y lista de corredores.
 */
class EngineGUI {
public:
  /**
   * @brief Premade visual themes for the GUI.
   */
  enum class Theme { Grey = 0, Dark = 1, VectonautaEngine = 2 };

  /**
   * @brief Default constructor (no-op).
   */
  EngineGUI() = default;

  /**
   * @brief Default destructor.
   */
  ~EngineGUI() = default;

  /**
   * @brief Initializes ImGui-SFML with the given window and sets default theme.
   * @param window Render window wrapper.
   */
  void init(const EngineUtilities::TSharedPointer<Window>& window);

  /**
   * @brief Per-frame GUI update and widget building.
   * @param window Render window wrapper.
   * @param deltaTime Time since last frame.
   * @param raceTimer Timer acumulado de la carrera (para mostrar en HUD).
   */
  void update(const EngineUtilities::TSharedPointer<Window>& window,
    sf::Time deltaTime,
    float raceTimer);

  /**
   * @brief Renders the ImGui draw data to the window.
   * @param window Render window wrapper.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window);

  /**
   * @brief Shuts down ImGui-SFML and releases GUI resources.
   */
  void destroy();

  /**
   * @brief Forwards SFML events to ImGui (keyboard/mouse/etc.).
   * @param window Render window wrapper.
   * @param event SFML event.
   */
  void processEvent(const EngineUtilities::TSharedPointer<Window>& window,
    const sf::Event& event);

  /**
   * @brief Returns whether the user requested to quit (File → Exit / botón).
   */
  bool shouldQuit() const { return m_requestQuit; }

  /**
   * @brief Returns current pause state toggled from the GUI.
   */
  bool isPaused() const { return m_paused; }

  /**
   * @brief Returns true once if a full reset was requested from the GUI; consumes the flag.
   */
  bool shouldResetWaypoints()
  {
    if (m_requestReset) { m_requestReset = false; return true; }
    return false;
  }

  /**
   * @brief Speed multiplier applied to simulation (0.1x–3.0x).
   */
  float getSpeedMultiplier() const { return m_speedMultiplier; }

  /**
   * @brief Sets the list of racers to be displayed/controlled in GUI.
   * @param racers Vector de corredores compartidos.
   */
  void setRacers(const std::vector<EngineUtilities::TSharedPointer<A_Racer>>& racers)
  {
    m_racers = racers;
  }

  /**
   * @brief Applies a different GUI theme (colors, rounding).
   * @param theme Theme enum value.
   */
  void setTheme(Theme theme);

private:
  /**
   * @brief Builds the main menu bar (File/Game/Theme).
   */
  void renderMenuBar();

  /**
   * @brief Builds the side control panel (pause, reset, speed, exit).
   */
  void renderControlPanel();

  /**
   * @brief Applies a light/grey GUI style preset.
   */
  void setupGreyGUIStyle();

  /**
   * @brief Applies a dark GUI style preset.
   */
  void setupDarkGUIStyle();

  /**
   * @brief Applies the custom VectonautaEngine GUI style preset.
   */
  void setupVectonautaEngineStyle();

  /** @brief Exit requested from GUI. */
  bool m_requestQuit = false;

  /** @brief Reset requested from GUI (consumible). */
  bool m_requestReset = false;

  /** @brief Pause toggle controlled from GUI. */
  bool m_paused = false;

  /** @brief Simulation speed multiplier controlled from GUI. */
  float m_speedMultiplier = 1.f;

  /** @brief Current GUI theme. */
  Theme m_currentTheme = Theme::VectonautaEngine;

  /** @brief Racers shown/operated in GUI panels. */
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;
};
