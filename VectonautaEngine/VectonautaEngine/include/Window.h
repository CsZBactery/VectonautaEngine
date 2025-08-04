#pragma once

#include "Prerequisites.h"
#include "Memory/TUniquePtr.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> // sf::RenderWindow, sf::Drawable, sf::Color
#include <functional>
#include <string>

/**
 * @file Window.h
 * @brief Wrapper ligero alrededor de sf::RenderWindow (SFML 3) con utilidades comunes.
 */
class Window {
public:
  Window() = default;

  /**
   * @brief Crea la ventana de una vez.
   */
  Window(int width, int height, const std::string& title);

  ~Window();

  /**
   * @brief Procesa los eventos pendientes.
   * @param callback Función opcional que se llama por cada evento.
   */
  void handleEvents(const std::function<void(const sf::Event&)>& callback = nullptr);

  /// @return true si la ventana sigue abierta.
  bool isOpen() const;

  /// Limpia con el color dado.
  void clear(const sf::Color& color = sf::Color::Black);

  /**
   * @brief Dibuja un drawable.
   */
  void draw(const sf::Drawable& drawable,
    const sf::RenderStates& states = sf::RenderStates::Default);

  /// Intercambia buffers / muestra en pantalla.
  void display();

  /// Actualiza deltaTime interno.
  void update();

  /// Paso de render extra (placeholder).
  void render();

  /// Cierra la ventana.
  void close();

  /// Limpia recursos.
  void destroy();

  /// Acceso al sf::RenderWindow subyacente.
  sf::RenderWindow& getInternal();

  /// Tiempo entre frames, actualizado en update().
  sf::Time deltaTime;

private:
  sf::View                                    m_view;
  EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr;
  sf::Clock   clock;
};
