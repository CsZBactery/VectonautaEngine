#include "BaseApp.h"
#include "Prerequisites.h"
#include "Window.h"
#include "EngineGUI.h"
#include "CShape.h"
#include "ECS/Transform.h"
#include "ECS/Actor.h"
#include <cmath>
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>

BaseApp::~BaseApp() {}

int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp", "run", "Initialization failed", "Check init() logic");
    return -1;
  }

  while (m_windowPtr->isOpen()) {
    // 1. Eventos: pasar cada evento al GUI y manejar cierre
    m_windowPtr->handleEvents([&](const sf::Event& event) {
      gui.processEvent(m_windowPtr, event); // ahora con la nueva firma

      if (event.is<sf::Event::Closed>()) {
        m_windowPtr->close();
      }
      else if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (static_cast<int>(key->scancode) == static_cast<int>(sf::Keyboard::Scancode::Escape)) {
          m_windowPtr->close();
        }
      }
      });

    // 2. Cerrar si se pidió desde el menú
    if (gui.shouldQuit()) {
      m_windowPtr->close();
    }

    // 3. Actualizar tiempo y GUI
    m_windowPtr->update();
    float dt = m_windowPtr->deltaTime.asSeconds();
    gui.update(m_windowPtr, m_windowPtr->deltaTime);

    // 4. Resetear waypoints si se pidió
    if (gui.shouldResetWaypoints()) {
      m_currentWaypointIndex = 0;
    }

    // 5. Lógica de movimiento de Mario (si no está en pausa)
    if (!gui.isPaused() && !m_circleActor.isNull()) {
      if (auto xf = m_circleActor->getComponent<Transform>()) {
        auto target = m_waypoints[m_currentWaypointIndex];
        auto pos = xf->getPosition();
        float dx = target.x - pos.x;
        float dy = target.y - pos.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist < 10.f) {
          if (++m_currentWaypointIndex >= static_cast<int>(m_waypoints.size()))
            m_currentWaypointIndex = 0;
        }
        float baseSpeed = 200.f;
        xf->seek(target, baseSpeed * gui.getSpeedMultiplier(), dt, 10.f);
      }
      m_circleActor->update(dt);
    }

    // 6. Actualizar pista si aplica
    if (!m_trackActor.isNull()) {
      m_trackActor->update(dt);
    }

    // 7. Renderizado
    m_windowPtr->clear(sf::Color::Black);

    if (!m_trackActor.isNull()) m_trackActor->render(m_windowPtr);
    if (!m_circleActor.isNull()) m_circleActor->render(m_windowPtr);

    // Panel adicional con info de Mario
    ImGui::Begin("Mario Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (auto xf = m_circleActor->getComponent<Transform>()) {
      auto pos = xf->getPosition();
      ImGui::Text("Position: %.1f, %.1f", pos.x, pos.y);
    }
    ImGui::Text("Current waypoint: %d", m_currentWaypointIndex);
    ImGui::End();

    gui.render(m_windowPtr);
    m_windowPtr->display();
  }

  destroy();
  return 0;
}

bool BaseApp::init() {
  // 1) Crear ventana
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "VectonautaEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window", "Check memory allocation");
    return false;
  }

  // 2) Inicializar GUI
  gui.init(m_windowPtr);

  // 3) Cargar textura y crear actor de la pista (fondo)
  if (!resourceMan.loadTexture("Sprites/Track", "png")) {
    MESSAGE("BaseApp", "init", "Cannot load Track.png");
  }
  auto trackTex = resourceMan.getTexture("Sprites/Track");
  if (trackTex.isNull()) {
    ERROR("BaseApp", "init", "Track texture null");
    return false;
  }

  m_trackActor = EngineUtilities::MakeShared<Actor>("Track");
  if (auto shape = m_trackActor->getComponent<CShape>()) {
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
  else {
    ERROR("BaseApp", "init", "Failed to get CShape from track actor");
    return false;
  }
  m_trackActor->setTexture(trackTex);
  if (auto xf = m_trackActor->getComponent<Transform>()) {
    xf->setPosition({ 0.f, 0.f });
  }

  // 4) Crear y configurar actor de Mario
  m_circleActor = EngineUtilities::MakeShared<Actor>("Mario Actor");
  if (m_circleActor.isNull()) {
    ERROR("BaseApp", "init", "Failed to create Mario actor", "");
    return false;
  }

  if (auto shape = m_circleActor->getComponent<CShape>()) {
    shape->createShape(ShapeType::CIRCLE);
    shape->setFillColor(sf::Color::White);
  }
  else {
    ERROR("BaseApp", "init", "Mario actor missing CShape");
    return false;
  }

  if (auto xf = m_circleActor->getComponent<Transform>()) {
    xf->setPosition({ 100.f, 150.f });
    xf->setScale({ 3.f, 3.f });
  }

  if (!resourceMan.loadTexture("Sprites/Mario", "png")) {
    MESSAGE("BaseApp", "init", "Cannot load Mario.png");
  }
  auto marioTex = resourceMan.getTexture("Sprites/Mario");
  if (marioTex.isNull()) {
    ERROR("BaseApp", "init", "Mario texture null");
    return false;
  }
  m_circleActor->setTexture(marioTex);

  // Waypoints para movimiento
  m_waypoints = {
    {400.f, 150.f},
    {700.f, 300.f},
    {1000.f, 150.f},
    {1200.f, 500.f}
  };
  m_currentWaypointIndex = 0;

  return true;
}

void BaseApp::update() {
  // Lógica se maneja dentro de run()
}

void BaseApp::render() {
  // Render dentro de run()
}

void BaseApp::destroy() {
  // Limpieza adicional si hace falta
}
