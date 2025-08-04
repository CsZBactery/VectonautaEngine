#include "Window.h"
#include <iostream> // para mensajes de error o debug

// Constructor: crea la ventana
Window::Window(int width, int height, const std::string& title) {
  m_windowPtr = EngineUtilities::MakeUnique<sf::RenderWindow>(
    sf::VideoMode({ static_cast<unsigned>(width), static_cast<unsigned>(height) }),
    title
  );

  if (m_windowPtr && m_windowPtr->isOpen()) {
    m_windowPtr->setFramerateLimit(60);
    // Inicializa vista si necesitas (opcional)
    m_view = m_windowPtr->getDefaultView();
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
    if (callback) callback(*event);

    if (event->is<sf::Event::Closed>()) {
      close();
    }
  }
}

bool Window::isOpen() const {
  if (!m_windowPtr) return false;
  return m_windowPtr->isOpen();
}

void Window::clear(const sf::Color& color) {
  if (!m_windowPtr) return;
  m_windowPtr->clear(color);
}

void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
  if (!m_windowPtr) return;
  m_windowPtr->draw(drawable, states);
}

void Window::display() {
  if (!m_windowPtr) return;
  m_windowPtr->display();
}

void Window::update() {
  if (!m_windowPtr) return;
  deltaTime = clock.restart();
}

void Window::render() {
  // Placeholder si necesitas dibujar overlays propios; actualmente no hace nada.
}

void Window::close() {
  if (m_windowPtr && m_windowPtr->isOpen()) {
    m_windowPtr->close();
  }
}

void Window::destroy() {
  if (m_windowPtr) {
    if (m_windowPtr->isOpen()) m_windowPtr->close();
    m_windowPtr.reset();
  }
}

sf::RenderWindow& Window::getInternal() {
  // Asume que siempre fue creado antes de llamar; si no, puedes hacer un assert o fallback.
  return *m_windowPtr;
}
