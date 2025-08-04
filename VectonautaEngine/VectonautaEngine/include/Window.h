#pragma once

#include "Prerequisites.h"
#include "Memory/TUniquePtr.h"
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> // por sf::RenderWindow, drawable, etc.
#include <functional>

/**
 * @file Window.h
 * @brief Wrapper around SFML RenderWindow for window management and rendering.
 */
class Window {
public:
  Window() = default;
  Window(int width, int height, const std::string& title);
  ~Window();

  // Ahora toma callback por evento
  void handleEvents(const std::function<void(const sf::Event&)>& callback = nullptr);

  bool isOpen() const;
  void clear(const sf::Color& color = sf::Color::Black);
  void draw(const sf::Drawable& drawable,
    const sf::RenderStates& states = sf::RenderStates::Default);
  void display();
  void destroy();
  void update();
  void render();

  void close(); // cerrar explícitamente

  // Exponer el sf::RenderWindow subyacente (para ImGui-SFML)
  sf::RenderWindow& getInternal() { return *m_windowPtr; }

private:
  sf::View                                    m_view;
public:
  EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr;
  sf::Clock   clock;
  sf::Time    deltaTime;
};
