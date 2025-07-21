#include "Actor.h"
#include "CShape.h"
#include "Transform.h"
#include "Texture.h

Actor::Actor(const std::string& actorName) {
  m_name = actorName;

  auto shape = EngineUtilities::MakeShared<CShape>();
  addComponent(shape);

  auto transform = EngineUtilities::MakeShared<Transform>();
  addComponent(transform);
}

void Actor::start() {
  // nada que hacer aún
}

void Actor::destroy() {
  // nada que hacer aún
}

void Actor::update(float deltaTime) {
  auto transform = getComponent<Transform>();
  auto shape = getComponent<CShape>();

  if (transform && shape) {
    shape->setPosition(transform->getPosition());
    shape->setRotation(transform->getRotation().x);
    shape->setScale(transform->getScale());
  }
}

void Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {
  for (auto& comp : components) {
    auto shape = comp.template dynamic_pointer_cast<CShape>();
    if (shape) {
      shape->render(window);
    }
  }
}

void
Actor::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  auto shape = getComponent<CShape>();
  if (shape) {
    if (!texture.isNull()) {
      shape->setTexture(texture);
      addComponent(texture);
    }
  }
