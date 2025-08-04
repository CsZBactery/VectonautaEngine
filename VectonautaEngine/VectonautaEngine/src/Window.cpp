#include "Prerequisites.h"
#include "Window.h"
#include <SFML/Graphics.hpp>
#include <functional> // por si no viene transitivamente

/**
 * @file Window.cpp
 * @brief Implements the Window wrapper for SFML RenderWindow.
 */

Window::Window(int width, int height, const std::string& title) {
  // Crear la ventana usando SFML 3: VideoMode toma sf::Vector2u explícito
  m_windowPtr = EngineUtilities::MakeUnique<sf::RenderWindow>(
    sf::VideoMode(sf::Vector2u(static_cast<unsigned>(width), static_cast<unsigned>(height))),
    title
  );

  if (m_windowPtr && m_windowPtr->isOpen()) {
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

void Window::handleEvents(const std::function<void(const sf::Event&)>& callback) {
  if (!m_windowPtr) return;

  // SFML 3: pollEvent devuelve std::optional<sf::Event>
  while (auto event = m_windowPtr->pollEvent()) {
    // Pasar evento al callback (por ejemplo GUI)
    if (callback) callback(*event);

    // Cerrar con el evento de ventana
    if (event->is<sf::Event::Closed>()) {
      close();
    }
    // Escapar con Escape
    else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
      if (static_cast<int>(key->scancode) == static_cast<int>(sf::Keyboard::Scancode::Escape)) {
        close();
      }
    }
  }
}

void Window::close() {
  if (m_windowPtr && m_windowPtr->isOpen()) {
    m_windowPtr->close();
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
