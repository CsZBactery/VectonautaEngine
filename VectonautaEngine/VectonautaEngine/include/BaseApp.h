#pragma once

/**
 * @file BaseApp.h
 * @brief Defines the BaseApp class, which manages the main application loop and rendering.
 */

#include "Prerequisites.h"
#include <Window.h>
#include "CShape.h" // Agregado para que coincida con el código del profesor

 /**
  * @class BaseApp
  * @brief Core application class that controls initialization, the main loop, rendering, and cleanup.
  */
class
  BaseApp {
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
  EngineUtilities::TSharedPointer<Window> m_windowPtr;   ///< Pointer to custom Window class using smart pointer.
  EngineUtilities::TSharedPointer<CShape> m_shapePtr;    ///< Pointer to custom shape class using smart pointer.
};