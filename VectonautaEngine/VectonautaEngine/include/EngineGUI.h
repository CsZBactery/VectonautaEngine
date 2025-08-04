#pragma once

#include "Prerequisites.h"
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

class Window;

class EngineGUI {
public:
  EngineGUI() = default;
  ~EngineGUI() = default;

  void init(const EngineUtilities::TSharedPointer<Window>& window);
  void update(const EngineUtilities::TSharedPointer<Window>& window, sf::Time deltaTime);
  void render(const EngineUtilities::TSharedPointer<Window>& window);
  void destroy();

  void setupGreyGUIStyle();
  void setupDarkGUIStyle();
  void renderMenuBar();

  void processEvent(const sf::Event& event);

  bool shouldQuit() const { return m_requestQuit; }

private:
  bool m_requestQuit = false; // Se pone true si el usuario pide salir desde el menú
};
