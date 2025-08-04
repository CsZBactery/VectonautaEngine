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
#include <ECS/Actor.h>

#include <vector>
#include <SFML/System.hpp> // para sf::Vector2f

 /**
  * @class BaseApp
  * @brief Core application class that controls initialization, the main loop, rendering, and cleanup.
  */
class BaseApp {
public:
  BaseApp() = default;
  ~BaseApp();

  /**
   * @brief Runs the application.
   * @return Exit code of the application.
   */
  int run();

  /**
   * @brief Initializes the application window and objects.
   * @return True if initialization was successful, false otherwise.
   */
  bool init();

  /**
   * @brief Updates the application logic (called every frame).
   */
  void update();

  /**
   * @brief Renders all drawable objects to the screen.
   */
  void render();

  /**
   * @brief Releases all allocated resources and cleans up.
   */
  void destroy();

private:
  EngineUtilities::TSharedPointer<Window> m_windowPtr;   ///< Ventana
  EngineGUI                               gui;          ///< Interfaz gráfica
  EngineUtilities::TSharedPointer<Actor>  m_circleActor;///< Actor de Mario
  EngineUtilities::TSharedPointer<Actor>  m_trackActor; ///< Actor de la pista

  ResourceManager                         resourceMan;  ///< Gestor de recursos
  std::vector<sf::Vector2f>               m_waypoints;  ///< Posiciones a seguir por el actor.
  int                                     m_currentWaypointIndex = 0; ///< Índice del waypoint.
};
