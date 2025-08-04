#include "Actor.h"
#include "Window.h"

void Actor::update(float deltaTime) {
  auto transform = getComponent<Transform>();
  auto shape = getComponent<CShape>();

  if (transform && shape) {
    // Aplica el transform guardado al shape
    if (sf::Shape* rawShape = shape->getShape()) {
      // Como sf::Shape hereda de Transformable, se aplican manualmente
      rawShape->setPosition(transform->getPosition());
      rawShape->setRotation(sf::degrees(transform->getRotation()));
      rawShape->setScale(transform->getScale());
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
