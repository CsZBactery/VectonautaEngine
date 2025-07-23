#include "Prerequisites.h"
#include "BaseApp.h"
#include "ResourceManager.h" 
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "CShape.h"
#include <cmath>  



/**
 * @file BaseApp.cpp
 * @brief Implements the BaseApp class which manages the main application loop.
 */

BaseApp::~BaseApp() {}

// Ejecuta el ciclo principal
int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp", "run", "Initialization failed", "Check init() logic");
  }

  while (m_windowPtr->isOpen()) {
    m_windowPtr->handleEvents();
    update();
    render();
  }

  destroy();
  return 0;
}

// Inicializa la ventana y los actores
bool BaseApp::init() {
  // 1) Crear ventana
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "VectonautaEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window", "Check memory allocation");
    return false;
  }

  // 2) Cargar textura y crear actor de la pista
  if (!resourceMan.loadTexture("Sprites/Track", "png")) {
    MESSAGE("BaseApp", "init", "Cannot load Track.png");
  }
  auto trackTex = resourceMan.getTexture("Sprites/Track");

  m_trackActor = EngineUtilities::MakeShared<Actor>("Track");
  if (auto shape = m_trackActor->getComponent<CShape>()) {
    // Creamos un RECTANGLE y lo preparamos
    shape->createShape(ShapeType::RECTANGLE);
    shape->setFillColor(sf::Color::White);

    // Ajustamos el tamaño del rectángulo al de la textura
    auto texSize = trackTex->getTexture().getSize();
    if (auto rect = dynamic_cast<sf::RectangleShape*>(shape->getShape())) {
      rect->setSize({ float(texSize.x), float(texSize.y) });
      rect->setOrigin(0.f, 0.f);
    }

    // Lo escalamos para cubrir toda la ventana (1920×1080)
    float scaleX = 1920.f / float(texSize.x);
    float scaleY = 1080.f / float(texSize.y);
    shape->setScale({ scaleX, scaleY });
  }
  m_trackActor->setTexture(trackTex);
  if (auto xf = m_trackActor->getComponent<Transform>()) {
    xf->setPosition({ 0.f, 0.f });
  }

  // 3) Crear y configurar actor de Mario
  m_circleActor = EngineUtilities::MakeShared<Actor>("Mario Actor");
  if (m_circleActor) {
    if (auto shape = m_circleActor->getComponent<CShape>()) {
      shape->createShape(ShapeType::CIRCLE);
      shape->setFillColor(sf::Color::White);
    }
    if (auto xf = m_circleActor->getComponent<Transform>()) {
      xf->setPosition({ 100.f, 150.f });
      xf->setScale({ 3.f, 3.f });
    }
    if (!resourceMan.loadTexture("Sprites/Mario", "png")) {
      MESSAGE("BaseApp", "init", "Cannot load Mario.png");
    }
    m_circleActor->setTexture(resourceMan.getTexture("Sprites/Mario"));

    // Waypoints para movimiento
    m_waypoints = {
        {400.f, 150.f},
        {700.f, 300.f},
        {1000.f, 150.f},
        {1200.f, 500.f}
    };
    m_currentWaypointIndex = 0;
  }
  else {
    ERROR("BaseApp", "init", "Failed to create Mario actor", "");
    return false;
  }

  return true;
}

// Actualiza la lógica de la aplicación cada frame
void BaseApp::update() {
  float dt = m_windowPtr->deltaTime.asSeconds();
  m_windowPtr->update();

  if (!m_trackActor.isNull()) {
    m_trackActor->update(dt);
  }

  if (!m_circleActor.isNull() && !m_waypoints.empty()) {
    m_circleActor->update(dt);

    auto xf = m_circleActor->getComponent<Transform>();
    auto target = m_waypoints[m_currentWaypointIndex];
    auto pos = xf->getPosition();

    float dx = target.x - pos.x;
    float dy = target.y - pos.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < 10.f) {
      if (++m_currentWaypointIndex >= static_cast<int>(m_waypoints.size()))
        m_currentWaypointIndex = 0;
    }
    xf->seek(target, 200.f, dt, 10.f);
  }
}

// Renderiza la pista y los actores
void BaseApp::render() {
  m_windowPtr->clear();

  if (!m_trackActor.isNull())
    m_trackActor->render(m_windowPtr);

  if (!m_circleActor.isNull())
    m_circleActor->render(m_windowPtr);

  m_windowPtr->display();
}

// Limpia recursos (los smart pointers liberan automáticamente)
void BaseApp::destroy() {
  // Nothing to explicitly delete
}