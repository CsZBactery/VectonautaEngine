#include "Prerequisites.h"
#include "Window.h"
#include <SFML/Graphics.hpp>

/**
 * @file Window.cpp
 * @brief Implements the Window wrapper for SFML RenderWindow.
 */

Window::Window(int width, int height, const std::string& title) {
  // Creamos el TUniquePtr correctamente
  m_windowPtr = EngineUtilities::MakeUnique<sf::RenderWindow>(
    sf::VideoMode(width, height),
    title
  );

  // Ahora sí podemos usarlo sin problemas
  if (m_windowPtr->isOpen()) {
    m_windowPtr->setFramerateLimit(60);
    MESSAGE("Window", "Window", "Created successfully");
  }
  else {
    ERROR("Window", "Window", "Failed to create RenderWindow");
  }
}

Window::~Window() {
  destroy();
}

void Window::handleEvents() {
  if (!m_windowPtr) {
    ERROR("Window", "handleEvents", "Window is null");
    return;
  }
  sf::Event event;
  while (m_windowPtr->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      m_windowPtr->close();
    }
  }
}

bool Window::isOpen() const {
  if (!m_windowPtr) {
    ERROR("Window", "isOpen", "Window is null");
    return false;
  }
  return m_windowPtr->isOpen();
}

void Window::clear(const sf::Color& color) {
  if (!m_windowPtr) {
    ERROR("Window", "clear", "Window is null");
    return;
  }
  m_windowPtr->clear(color);
}

void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
  if (!m_windowPtr) {
    ERROR("Window", "draw", "Window is null");
    return;
  }
  m_windowPtr->draw(drawable, states);
}

void Window::display() {
  if (!m_windowPtr) {
    ERROR("Window", "display", "Window is null");
    return;
  }
  m_windowPtr->display();
}

void Window::update() {
  deltaTime = clock.restart();
}

void Window::render() {
  // placeholder, p. ej. ImGui
}

void Window::destroy() {
  if (m_windowPtr) {
    if (m_windowPtr->isOpen())
      m_windowPtr->close();
    m_windowPtr.reset();  // libera correctamente
  }
}
