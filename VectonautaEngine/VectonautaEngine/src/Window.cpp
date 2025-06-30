#include "Window.h"
#include <BaseApp.h>

/**
 * @class Window
 *
 * @brief Encapsulates an SFML window, handling creation, events, rendering, and destruction.
 */

 /**
  * @brief Constructs a new Window object.
  *
  * Initializes an SFML render window with the specified width, height, and title.
  * It also sets the framerate limit and verifies successful creation.
  *
  * @param width Width of the window in pixels.
  * @param height Height of the window in pixels.
  * @param title Title of the window.
  */
Window::Window(int width, int height, const std::string& title) {
  m_windowPtr = EngineUtilities::MakeUnique<sf::RenderWindow>(
    sf::VideoMode(width, height), title);

  if (!m_windowPtr.isNull()) {
    m_windowPtr->setFramerateLimit(60);
    MESSAGE("Window", "Window", "Window created successfully");
  }
  else {
    ERROR("Window", "Window", "Failed to create window");
  }
}

/**
 * @brief Destroys the Window object and safely releases its resources.
 */
Window::~Window() {
  m_windowPtr.release();
}

/**
 * @brief Handles window events such as closing.
 *
 * Processes the event queue to detect and handle user actions like closing the window.
 */
void Window::handleEvents() {
  sf::Event event;
  while (m_windowPtr->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      m_windowPtr->close();
    }
  }
}

/**
 * @brief Checks if the window is currently open.
 *
 * @return true if the window is open, false otherwise.
 */
bool Window::isOpen() const {
  if (!m_windowPtr.isNull()) {
    return m_windowPtr->isOpen();
  }
  else {
    ERROR("Window", "isOpen", "Window is null");
    return false;
  }
}

/**
 * @brief Clears the window with a specific background color.
 *
 * @param color The color to use when clearing the window.
 */
void Window::clear(const sf::Color& color) {
  if (!m_windowPtr.isNull()) {
    m_windowPtr->clear(color);
  }
  else {
    ERROR("Window", "clear", "Window is null");
  }
}

/**
 * @brief Draws a drawable object to the window using specified render states.
 *
 * @param drawable The SFML drawable object to render.
 * @param states Optional render states to apply to the drawable.
 */
void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
  if (!m_windowPtr.isNull()) {
    m_windowPtr->draw(drawable, states);
  }
  else {
    ERROR("Window", "draw", "Window is null");
  }
}

/**
 * @brief Displays the contents of the current frame on the screen.
 */
void Window::display() {
  if (!m_windowPtr.isNull()) {
    m_windowPtr->display();
  }
  else {
    ERROR("Window", "display", "Window is null");
  }
}

/**
 * @brief Destroys the window and releases its resources safely.
 */
void Window::destroy() {
  m_windowPtr.release();
}