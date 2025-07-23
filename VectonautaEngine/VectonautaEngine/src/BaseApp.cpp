#include "Prerequisites.h"
#include "BaseApp.h"
#include "ResourceManager.h" 
#include "ECS/Actor.h"
#include "ECS/Transform.h"    // <— asegúrate de incluir Transform
#include <cmath>  



/**
 * @file BaseApp.cpp
 * @brief Implements the BaseApp class which manages the main application loop.
 */

 /**
  * @brief Destructor for BaseApp.
  *
  * Currently empty, as resources are freed in the destroy() method.
  */
BaseApp::~BaseApp() {
}

// Ejecuta el ciclo principal
int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp", "run", "Initializes result on a false statement",
      "check method validations");
  }

  while (m_windowPtr->isOpen()) {
    m_windowPtr->handleEvents();
    update();
    render();
  }

  destroy();
  
}

// Inicializa la ventana y los actores
bool BaseApp::init() {
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "VectonautaEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window pointer", "check memory allocation");
    return false;
  }

  m_shapePtr = EngineUtilities::MakeShared<CShape>();
  if (m_shapePtr) {
    m_shapePtr->createShape(ShapeType::CIRCLE);
    m_shapePtr->setFillColor(sf::Color::Green);
    m_shapePtr->setPosition(200.f, 150.f);
  }

  /*m_shapePtr = EngineUtilities::MakeShared<CShape>();
  if (m_shapePtr) {
    m_shapePtr->createShape(ShapeType::CIRCLE);
    m_shapePtr->setFillColor(sf::Color::Blue);
    m_shapePtr->setPosition(150.f, 200.f);
  }*/

  // Crear el Actor
  m_circleActor = EngineUtilities::MakeShared<Actor>("Circle Actor");
  if (m_circleActor) { 
    auto shape = m_circleActor->getComponent<CShape>();
    if (shape) {
      shape->createShape(ShapeType::CIRCLE);
      shape->setFillColor(sf::Color::White);
    }

    auto transform = m_circleActor->getComponent<Transform>();
    if (transform) {
      transform->setPosition(sf::Vector2f(100.f, 150.f));
      transform->setScale({ 3.f, 3.f });
    }

    //Cargar la textura para el actor
    if (!resourceMan.loadTexture("Sprites/Mario", "png")) {
      MESSAGE("BaseApp", "Init", "Can´t load the texture")
    }

    m_circleActor->setTexture(resourceMan.getTexture("Sprites/Mario"));
    //m_circleActor->setName("Circle Actor)

    m_waypoints = {
        {400.f, 150.f},
        {700.f, 300.f},
        {1000.f, 150.f},
        {1200.f, 500.f}
    };
    m_currentWaypointIndex = 0;
  }
  else {
    ERROR("BaseApp", "init", "Failed to create Circle Actor", "");
    return false;
  }

  return true;
}

/**
 * @brief Updates the application state.
 *
 * Empty for now. Intended for game logic or state updates.
 */

void BaseApp::update() {
  if (!m_windowPtr.isNull()) {
    m_windowPtr->update();
  }

  // Update Actors
  if (!m_circleActor.isNull() && !m_waypoints.empty()) {  // <— paréntesis corregidos
    m_circleActor->update(m_windowPtr->deltaTime.asSeconds());

    sf::Vector2f targetPos = m_waypoints[m_currentWaypointIndex];
    sf::Vector2f currentPos = m_circleActor->getComponent<Transform>()->getPosition();

    float dx = targetPos.x - currentPos.x;
    float dy = targetPos.y - currentPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 10.f) {
      if (++m_currentWaypointIndex >= static_cast<int>(m_waypoints.size()))
        m_currentWaypointIndex = 0;
    }

    // Busca hacia el waypoint actual
    m_circleActor->getComponent<Transform>()->seek(
      m_waypoints[m_currentWaypointIndex], 
      200.f,
      m_windowPtr->deltaTime.asSeconds(),
      10.f
    );
  }
}

/**
 * @brief Renders the current frame.
 *
 * Clears the screen, draws the circle shape, and displays the result.
 */

// Render por frame
void BaseApp::render() {
  if (!m_windowPtr) return;

  m_windowPtr->clear();

  if (m_shapePtr) {
    m_shapePtr->render(m_windowPtr);
  }

  if (!m_circleActor.isNull()) {
    m_circleActor->render(m_windowPtr);
  }

  m_windowPtr->display();
}


/**
 * @brief Releases allocated resources.
 *
 * Deletes the shape and properly destroys the window.
 */

// Cleanup
void BaseApp::destroy() {
  // Smart pointers limpian automáticamente
  // m_shapePtr.Reset(); // Not necessary if using smart pointers correctly
  // m_windowPtr.Reset(); // Cleanup handled automatically
}
