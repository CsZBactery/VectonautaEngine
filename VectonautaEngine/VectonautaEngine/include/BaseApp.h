#pragma once

/**
 * @file BaseApp.h
 * @brief Defines the BaseApp class, which manages the main application loop and rendering.
 */

#include <Prerequisites.h>
#include <ResourceManager.h>
#include <Window.h>
#include "EngineGUI.h"
#include <CShape.h>
#include <ECS/Transform.h>
#include <ECS/Actor.h>
#include <A_Racer.h>

#include <vector>
#include <SFML/System.hpp>

 /**
  * @class BaseApp
  * @brief Core application class that controls initialization, the main loop, rendering, and cleanup.
  */
class BaseApp {
public:
  /**
   * @brief Default constructor.
   */
  BaseApp() = default;

  /**
   * @brief Destructor that handles cleanup.
   */
  ~BaseApp();

  /**
   * @brief Runs the application.
   * This method initializes the application, enters the main loop, and calls update/render methods.
   * @return Exit code of the application.
   */
  int  run();

  /**
   * @brief Initializes the application window and objects.
   * @return True if initialization was successful, false otherwise.
   */
  bool init();

  /**
   * @brief Updates the application logic (called every frame).
   */
  void update() {}

  /**
   * @brief Renders all drawable objects to the screen.
   */
  void render() {}

  /**
   * @brief Releases all allocated resources and cleans up.
   */
  void destroy() {}

private:
  EngineUtilities::TSharedPointer<Window>           m_windowPtr;
  EngineUtilities::TSharedPointer<Actor>            m_trackActor;
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_finishedOrder;

  ResourceManager   resourceMan;
  EngineGUI         gui;

  std::vector<sf::Vector2f> m_path;
  sf::FloatRect             m_finishLine;
  float                     m_raceTimer = 0.f;
  bool                      m_raceStarted = false;
};
