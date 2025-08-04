#include "BaseApp.h"
#include "Prerequisites.h"
#include "Window.h"
#include "EngineGUI.h"
#include "CShape.h"
#include "ECS/Transform.h"
#include "ECS/Actor.h"
#include <cmath>
#include <iostream>

BaseApp::~BaseApp() {}

int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp", "run", "Initialization failed", "Check init() logic");
    return -1;
  }

  while (m_windowPtr->isOpen()) {
    // 1. Eventos: pasar cada evento al GUI y manejar cierre
    m_windowPtr->handleEvents([&](const sf::Event& event) {
      // Procesar evento para ImGui-SFML (ahora con ventana + evento)
      ImGui::SFML::ProcessEvent(m_windowPtr->getInternal(), event);

      if (event.is<sf::Event::Closed>()) {
        m_windowPtr->close();
      }
      });


    // 2. Actualizar tiempo
    m_windowPtr->update();
    float dt = m_windowPtr->deltaTime.asSeconds();

    if (!m_trackActor.isNull()) m_trackActor->update(dt);
    if (!m_circleActor.isNull()) m_circleActor->update(dt);

    gui.update(m_windowPtr, m_windowPtr->deltaTime);

    if (gui.shouldQuit()) {
      m_windowPtr->close();
    }

    // 3. Renderizado
    m_windowPtr->clear(sf::Color::Black);

    if (!m_trackActor.isNull()) m_trackActor->render(m_windowPtr);
    if (!m_circleActor.isNull()) m_circleActor->render(m_windowPtr);

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

    // Ajustar tamaño para cubrir la ventana
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
  // Lógica específica ya se maneja en run()
}

void BaseApp::render() {
  // Render ya integrado en run()
}

void BaseApp::destroy() {
  // Limpieza adicional si se necesitara; smart pointers se encargan del resto
}
