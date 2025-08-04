#include "ECS/Actor.h"
#include "CShape.h"
#include "ECS/Transform.h"
#include "ECS/Texture.h"
#include "Window.h"

void Actor::update(float deltaTime) {
  auto transform = getComponent<Transform>();
  auto shape = getComponent<CShape>();

  if (transform && shape) {
    if (auto rawShape = shape->getShape()) {
      transform->applyTo(*rawShape);
    }
  }
}

void Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {
  for (const auto& comp : components) {
    if (auto shape = comp.template dynamic_pointer_cast<CShape>()) {
      shape->render(window);
    }
    else if (auto textureComp = comp.template dynamic_pointer_cast<Texture>()) {
      textureComp->render(window);
    }
  }
}

void Actor::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  if (texture.isNull()) return;
  if (auto shape = getComponent<CShape>()) {
    shape->setTexture(texture);
  }
  addComponent<Texture>(texture);
}
