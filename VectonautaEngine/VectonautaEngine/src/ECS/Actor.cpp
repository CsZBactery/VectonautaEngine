#include "ECS/Actor.h"
#include "CShape.h"
#include "Transform.h"
#include "Window.h"


Actor::Actor(const std::string& actorName) {
  // Setup Actor Name
  m_name = actorName;

  // Setup Shape
  EngineUtilities::TSharedPointer<CShape> shape = EngineUtilities::MakeShared<CShape>();
  addComponent(shape);

  // Setup Transform
  EngineUtilities::TSharedPointer<Transform> transform = EngineUtilities::MakeShared<Transform>();
  addComponent(transform);
}

void
Actor::update(float deltaTime) {
  auto transform = getComponent<Transform>();
  auto shape = getComponent<CShape>();

  if (transform && shape) {
    shape->setPosition(transform->getPosition());

    // Usar el ángulo X de rotación si lo defines así
    shape->SetRotation(transform->getRotation().x);

    shape->setScale(transform->getScale());
  }
}

void
Actor::render(const EngineUtilities::TSharedPointer<Window>& window) 
{
  for (auto& comp : components) {
    auto shape = comp->template dynamic_pointer_cast<CShape>();
    if (shape) {
      shape->render(window);
    }
  }
}
