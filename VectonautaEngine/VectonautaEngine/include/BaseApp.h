#pragma once

#include <Prerequisites.h>
#include <ResourceManager.h>
#include <Window.h>
#include "EngineGUI.h"
#include <CShape.h>
#include <ECS/Transform.h>
#include <ECS/Actor.h>
#include <A_Racer.h>

#include <vector>
#include <SFML/System.hpp>

class BaseApp {
public:
  BaseApp() = default;
  ~BaseApp();

  int run();
  bool init();
  void update();
  void render();
  void destroy();

private:
  EngineUtilities::TSharedPointer<Window> m_windowPtr;
  EngineUtilities::TSharedPointer<Actor> m_trackActor;
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_finishedOrder; // orden oficial

  ResourceManager resourceMan;
  EngineGUI gui;

  std::vector<sf::Vector2f> m_path;
  sf::FloatRect m_finishLine = {}; // línea/meta

  // (puedes mantener otros campos como waypoints previos si ya no los usas)
};
