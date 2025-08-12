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
  /**
   * @brief Constructor por defecto (no crea la ventana todavía).
   */
  Window() = default;

  /**
   * @brief Crea la ventana de una vez con tamaño y título.
   * @param width Ancho en píxeles.
   * @param height Alto en píxeles.
   * @param title Título de la ventana.
   */
  Window(int width, int height, const std::string& title);

  /**
   * @brief Destructor: libera la ventana y recursos asociados.
   */
  ~Window();

  /**
   * @brief Procesa los eventos pendientes de SFML.
   * @param callback Función opcional llamada por cada evento recibido.
   */
  void handleEvents(const std::function<void(const sf::Event&)>& callback = nullptr);

  /**
   * @brief Indica si la ventana sigue abierta.
   * @return true si está abierta, false si ya se cerró.
   */
  bool isOpen() const;

  /**
   * @brief Limpia el framebuffer con el color dado.
   * @param color Color de limpieza (por defecto negro).
   */
  void clear(const sf::Color& color = sf::Color::Black);

  /**
   * @brief Dibuja un objeto drawable en la ventana.
   * @param drawable Cualquier sf::Drawable (shape, sprite, text, etc.).
   * @param states Estados de render opcionales.
   */
  void draw(const sf::Drawable& drawable,
    const sf::RenderStates& states = sf::RenderStates::Default);

  /**
   * @brief Intercambia buffers y presenta en pantalla.
   */
  void display();

  /**
   * @brief Actualiza el delta time interno (debe llamarse una vez por frame).
   */
  void update();

  /**
   * @brief Paso de render adicional (placeholder para integraciones futuras).
   */
  void render();

  /**
   * @brief Solicita el cierre de la ventana.
   */
  void close();

  /**
   * @brief Libera recursos asociados a la ventana.
   */
  void destroy();

  /**
   * @brief Acceso al sf::RenderWindow subyacente.
   * @return Referencia al objeto de SFML.
   */
  sf::RenderWindow& getInternal();

  /**
   * @brief Tiempo entre frames, actualizado en update().
   */
  sf::Time deltaTime;

private:
  /**
   * @brief Vista actual usada para mapear coordenadas y dibujar.
   */
  sf::View m_view;

  /**
   * @brief Puntero único a la instancia de sf::RenderWindow.
   */
  EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr;

  /**
   * @brief Reloj interno para calcular deltaTime.
   */
  sf::Clock clock;
};
