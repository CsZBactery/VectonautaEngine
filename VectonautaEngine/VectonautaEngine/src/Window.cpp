#include "Window.h"
#include <iostream> // mensajes de error o debug

// Constructor: crea la ventana y configura FPS / vista por defecto
Window::Window(int width, int height, const std::string& title) {
  // Crea sf::RenderWindow dentro de un TUniquePtr (propiedad exclusiva del wrapper)
  m_windowPtr = EngineUtilities::MakeUnique<sf::RenderWindow>(
    sf::VideoMode({ static_cast<unsigned>(width), static_cast<unsigned>(height) }),
    title
  );

  // Si la creación fue exitosa, configura límites y vista inicial
  if (m_windowPtr && m_windowPtr->isOpen()) {
    m_windowPtr->setFramerateLimit(60);     // cap de FPS (más estable para debug)
    m_view = m_windowPtr->getDefaultView(); // guarda la vista por defecto
    MESSAGE("Window", "Window", "Created successfully");
  }
  else {
    ERROR("Window", "Window", "Failed to create RenderWindow");
  }
}

// Destructor: asegura limpieza ordenada
Window::~Window() {
  destroy();
}

// Bucle de eventos: despacha a callback y maneja cierre de ventana
void Window::handleEvents(const std::function<void(const sf::Event&)>& callback) {
  if (!m_windowPtr) return;

  // En SFML 3, pollEvent() retorna std::optional<sf::Event>
  while (auto event = m_windowPtr->pollEvent()) {
    // Si hay callback del usuario, se invoca con el evento actual
    if (callback) callback(*event);

    // Cierre de ventana solicitado
    if (event->is<sf::Event::Closed>()) {
      close();
    }
  }
}

// ¿La ventana sigue abierta?
bool Window::isOpen() const {
  if (!m_windowPtr) return false;
  return m_windowPtr->isOpen();
}

// Limpia el backbuffer con un color (negro por defecto)
void Window::clear(const sf::Color& color) {
  if (!m_windowPtr) return;
  m_windowPtr->clear(color);
}

// Dibuja cualquier sf::Drawable con estados opcionales
void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
  if (!m_windowPtr) return;
  m_windowPtr->draw(drawable, states);
}

// Presenta en pantalla el contenido del frame
void Window::display() {
  if (!m_windowPtr) return;
  m_windowPtr->display();
}

// Actualiza deltaTime usando un reloj interno; llamar una vez por frame
void Window::update() {
  if (!m_windowPtr) return;
  deltaTime = clock.restart(); // tiempo transcurrido desde el último frame
}

// Paso de render adicional (placeholder para overlays o debug propio)
void Window::render() {
  // Actualmente no hace nada; útil si quieres dibujar HUD fuera de ImGui.
}

// Solicita el cierre de la ventana (no destruye el objeto aún)
void Window::close() {
  if (m_windowPtr && m_windowPtr->isOpen()) {
    m_windowPtr->close();
  }
}

// Libera recursos asociados a la ventana y resetea el puntero
void Window::destroy() {
  if (m_windowPtr) {
    if (m_windowPtr->isOpen()) m_windowPtr->close();
    m_windowPtr.reset();
  }
}

// Acceso directo al sf::RenderWindow subyacente (referencia no nula asumida)
sf::RenderWindow& Window::getInternal() {
  // Si necesitas más seguridad, podrías añadir un assert aquí.
  return *m_windowPtr;
}
