#include <iostream>
#include <cmath>

#include "Memory/TUniquePtr.h"
#include "Memory/TSharedPointer.h"
#include "Window.h"
#include "EngineGUI.h"
#include "ResourceManager.h"
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "CShape.h"
#include <SFML/Graphics.hpp>

int main() {
  // === 1. Crear ventana ===
  auto windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "VectonautaEngine");
  if (!windowPtr) {
    std::cerr << "Error: no se pudo crear la ventana.\n";
    return -1;
  }

  // === 2. Inicializar GUI ===
  EngineGUI gui;
  gui.init(windowPtr);

  // === 3. Resource manager ===
  ResourceManager resourceMan;

  // === 4. Cargar pista (fondo) ===
  if (!resourceMan.loadTexture("Sprites/Track", "png")) {
    std::cerr << "[Main] No se pudo cargar Sprites/Track.png\n";
  }
  auto trackTex = resourceMan.getTexture("Sprites/Track");
  if (trackTex.isNull()) {
    std::cerr << "[Main] Track texture es nula\n";
  }

  EngineUtilities::TSharedPointer<Actor> trackActor = EngineUtilities::MakeShared<Actor>("Track");
  if (auto shape = trackActor->getComponent<CShape>()) {
    shape->createShape(ShapeType::RECTANGLE);
    shape->setFillColor(sf::Color::White);
    auto texSize = trackTex->getTexture().getSize();
    if (auto rect = dynamic_cast<sf::RectangleShape*>(shape->getShape())) {
      rect->setSize({ static_cast<float>(texSize.x), static_cast<float>(texSize.y) });
      rect->setOrigin({ 0.f, 0.f });
    }
    float scaleX = 1920.f / static_cast<float>(texSize.x);
    float scaleY = 1080.f / static_cast<float>(texSize.y);
    shape->setScale({ scaleX, scaleY });
  }
  trackActor->setTexture(trackTex);
  if (auto xf = trackActor->getComponent<Transform>()) {
    xf->setPosition({ 0.f, 0.f });
  }

  // === 5. Cargar Mario ===
  if (!resourceMan.loadTexture("Sprites/Mario", "png")) {
    std::cerr << "[Main] No se pudo cargar Sprites/Mario.png\n";
  }
  auto marioTex = resourceMan.getTexture("Sprites/Mario");
  if (marioTex.isNull()) {
    std::cerr << "[Main] Mario texture es nula\n";
  }

  EngineUtilities::TSharedPointer<Actor> marioActor = EngineUtilities::MakeShared<Actor>("Mario Actor");
  if (auto shape = marioActor->getComponent<CShape>()) {
    shape->createShape(ShapeType::CIRCLE);
    shape->setFillColor(sf::Color::White); // si quieres textura visible, puedes quitar fill o hacerlo transparente
  }
  if (auto xf = marioActor->getComponent<Transform>()) {
    xf->setPosition({ 100.f, 150.f });
    xf->setScale({ 3.f, 3.f });
  }
  marioActor->setTexture(marioTex);

  // === 6. Waypoints ===
  std::vector<sf::Vector2f> waypoints = {
    {400.f, 150.f},
    {700.f, 300.f},
    {1000.f, 150.f},
    {1200.f, 500.f}
  };
  int currentWaypointIndex = 0;

  // === Loop principal ===
  while (windowPtr->isOpen()) {
    // Eventos
    windowPtr->handleEvents([&](const sf::Event& event) {
      gui.processEvent(event);

      if (event.is<sf::Event::Closed>()) {
        windowPtr->close();
      }
      else if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (static_cast<int>(key->scancode) == static_cast<int>(sf::Keyboard::Scancode::Escape)) {
          windowPtr->close();
        }
      }
      });

    // Cerrar si el menú pidió salir
    if (gui.shouldQuit()) {
      windowPtr->close();
    }

    // Actualizar deltaTime y GUI
    windowPtr->update();
    float dt = windowPtr->deltaTime.asSeconds();
    gui.update(windowPtr, windowPtr->deltaTime);

    // Actualizar lógica de Mario: seguir waypoints
    if (!marioActor.isNull()) {
      if (auto xf = marioActor->getComponent<Transform>()) {
        auto target = waypoints[currentWaypointIndex];
        auto pos = xf->getPosition();
        float dx = target.x - pos.x;
        float dy = target.y - pos.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist < 10.f) {
          if (++currentWaypointIndex >= static_cast<int>(waypoints.size()))
            currentWaypointIndex = 0;
        }
        xf->seek(target, 200.f, dt, 10.f);
      }
      marioActor->update(dt);
    }

    // Actualizar fondo si es necesario
    if (!trackActor.isNull()) {
      trackActor->update(dt);
    }

    // Render
    windowPtr->clear(sf::Color::Black);

    if (!trackActor.isNull()) trackActor->render(windowPtr);
    if (!marioActor.isNull()) marioActor->render(windowPtr);

    // Panel extra en GUI con info de Mario
    ImGui::Begin("Mario Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (auto xf = marioActor->getComponent<Transform>()) {
      auto pos = xf->getPosition();
      ImGui::Text("Position: %.1f, %.1f", pos.x, pos.y);
    }
    ImGui::Text("Current waypoint: %d", currentWaypointIndex);
    ImGui::End();

    gui.render(windowPtr);

    windowPtr->display();
  }

  gui.destroy();
  return 0;
}
