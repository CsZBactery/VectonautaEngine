#pragma once
#include "Prerequisites.h"
#include "Memory/TSharedPointer.h"
#include "Memory/TUniquePtr.h"


/**
 * @file Window.h
 * @brief Declares the Window class, a wrapper for managing an SFML RenderWindow.
 */

 /**
  * @class Window
  * @brief Encapsulates an SFML window, handling initialization, events, rendering, and cleanup.
  */
class
  Window {
public:
  /**
   * @brief Default constructor.
   *
   * Does not create a window. Use this only if you plan to initialize the window later.
   */
  Window() = default;

  /**
   * @brief Constructs and initializes a new window.
   *
   * @param width Width of the window in pixels.
   * @param height Height of the window in pixels.
   * @param title Title of the window.
   */
  Window(int width, int height, const std::string& title);

  /**
   * @brief Destructor. Releases any allocated resources.
   */
  ~Window();

  /**
   * @brief Handles window events (e.g., close, input).
   *
   * Processes all SFML events in the queue.
   */
  void
    handleEvents();

  /**
   * @brief Checks if the window is currently open.
   *
   * @return true if the window is open, false otherwise.
   */
  bool
    isOpen() const;

  /**
   * @brief Clears the window with a specified color.
   *
   * @param color The color used to clear the screen. Defaults to black.
   */
  void
    clear(const sf::Color& color = sf::Color(0, 0, 0, 255));

  /**
   * @brief Draws a renderable object to the window.
   *
   * @param drawable The object to draw (e.g., shape, sprite, text).
   * @param states Optional render states. Defaults to sf::RenderStates::Default.
   */
  void
    draw(const sf::Drawable& drawable,
      const sf::RenderStates& states = sf::RenderStates::Default);

  /**
   * @brief Displays the contents of the window.
   *
   * Should be called after drawing all objects for the current frame.
   */
  void
    display();

  /**
   * @brief Releases the window resources.
   *
   * Properly deletes the internal SFML window pointer.
   */
  void
    destroy();

private:
  EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr; ///< Unique pointer to the SFML render window.
  sf::View m_view; ///< View used for rendering (not currently exposed).
};