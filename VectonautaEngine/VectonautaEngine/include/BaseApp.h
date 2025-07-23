#pragma once
/**
 * @file BaseApp.h
 * @brief Defines the BaseApp class, which manages the main application loop and rendering.
 */

#include "Prerequisites.h"
#include "ResourceManager.h"
#include "Window.h"
#include "CShape.h" 
#include "ECS/Actor.h"

#include <vector>
#include <SFML/System/Vector2.hpp> // para sf::Vector2f

 /**
  * @class BaseApp
  * @brief Core application class that controls initialization, the main loop, rendering, and cleanup.
  */
class BaseApp {
public:

  //Constructors

  /**
   * @brief Default constructor. Initializes the vector to (0, 0).
   */
  BaseApp() = default;

  /**
  * @brief Destructor that handles cleanup.
  */
  ~BaseApp();

  /**
   * @brief Runs the application.
   *
   * This method initializes the application, enters the main loop, and calls update/render methods.
   * @return Exit code of the application.
   */
  int
    run();

  /**
   * @brief Initializes the application window and objects.
   * @return True if initialization was successful, false otherwise.
   */
  bool
    init();

  /**
   * @brief Updates the application logic (called every frame).
   */
  void
    update();

  /**
   * @brief Renders all drawable objects to the screen.
   */
  void
    render();

  /**
   * @brief Releases all allocated resources and cleans up.
   */
  void
    destroy();

private:
  EngineUtilities::TSharedPointer<Window> m_windowPtr;   //Pointer to custom Window class.
  EngineUtilities::TSharedPointer<CShape> m_shapePtr;    //Pointer to custom shape class.
  EngineUtilities::TSharedPointer<Actor> m_circleActor;



  ResourceManager    resourceMan;
  std::vector<sf::Vector2f> m_waypoints; ///< Posiciones a seguir por el actor.
  int m_currentWaypointIndex = 0;        ///< Indice del waypoint.
 
};