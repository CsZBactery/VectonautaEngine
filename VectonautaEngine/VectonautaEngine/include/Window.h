#pragma once

#include "Prerequisites.h"              // Core utilities and SFML headers
#include "Memory/TUniquePtr.h"           // EngineUtilities::TUniquePtr
#include <string>                         // std::string
#include <SFML/Window.hpp>        // sf::Event

/**
 * @file Window.h
 * @brief Wrapper around SFML RenderWindow for window management and rendering.
 */
class Window {
public:
  /**
   * @brief Default constructor. Window must be created later via init function.
   */
  Window() = default;

  /**
   * @brief Constructs and creates the SFML window.
   * @param width  Width in pixels.
   * @param height Height in pixels.
   * @param title  Window title.
   */
  Window(int width, int height, const std::string& title);

  /**
   * @brief Destructor. Cleans up the SFML window.
   */
  ~Window();

  /**
   * @brief Polls and processes all pending SFML events.
   */
  void handleEvents();

  /**
   * @brief Checks if the window is still open.
   * @return true if open, false otherwise.
   */
  bool isOpen() const;

  /**
   * @brief Clears the window with the given color (default black).
   * @param color Clear color.
   */
  void clear(const sf::Color& color = sf::Color::Black);

  /**
   * @brief Draws an SFML drawable object.
   * @param drawable Object implementing sf::Drawable.
   * @param states   Optional render states.
   */
  void draw(const sf::Drawable& drawable,
    const sf::RenderStates& states = sf::RenderStates::Default);

  /**
   * @brief Displays the back buffer on the screen.
   */
  void display();

  /**
   * @brief Destroys the window and frees resources.
   */
  void destroy();

  /**
   * @brief Updates the deltaTime for frame timing.
   */
  void update();

  /**
   * @brief Optional render step placeholder for ImGui or other overlays.
   */
  void render();

private:
    ///< SFML window pointer
  sf::View                                    m_view;        ///< Current view

public:
EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr;
  sf::Clock   clock;      ///< Clock to measure frame time
  sf::Time    deltaTime;  ///< Time elapsed since last frame
};
