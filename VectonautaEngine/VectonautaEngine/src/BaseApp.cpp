#include "BaseApp.h"
#include "ECS/Actor.h"

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
  return 0;
}

// Inicializa la ventana y los actores
bool BaseApp::init() {
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "VectonautaEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window pointer, check memory allocation");
    return false;
  }

  // Crear el Actor
  m_circleActor = EngineUtilities::MakeShared<Actor>("Circle Actor");
  if (!m_circleActor) {
    ERROR("BaseApp", "init", "Failed to create Circle Actor");
    return false;
  }

  // Configurar CShape
  auto shape = m_circleActor->getComponent<CShape>();
  if (shape) {
    shape->createShape(ShapeType::CIRCLE);
    shape->setFillColor(sf::Color::Yellow);
  }

  // Configurar Transform
  auto transform = m_circleActor->getComponent<Transform>();
  if (transform) {
    transform->setPosition(sf::Vector2f(200.f, 150.f));
    transform->setRotation(sf::Vector2f(0.f, 0.f));
    transform->setScale(sf::Vector2f(1.f, 1.f));
  }

  return true;
}

// Lógica por frame
void BaseApp::update() {
  if (m_circleActor) {
    m_circleActor->update(0.f);
  }
}

// Render por frame
void BaseApp::render() {
  if (!m_windowPtr) return;

  m_windowPtr->clear();

  if (m_circleActor) {
    m_circleActor->render(m_windowPtr);
  }

  m_windowPtr->display();
}

// Cleanup
void BaseApp::destroy() {
  // Smart pointers limpian automáticamente
}
