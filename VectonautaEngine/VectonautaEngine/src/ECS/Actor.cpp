#include "ECS/Actor.h"
#include "CShape.h"
#include "ECS/Transform.h"
#include "ECS/Texture.h"
#include "Window.h"

void Actor::update(float dt) {
  (void)dt;
  auto xf = getComponent<Transform>();
  if (!xf) return;

  if (auto shape = getComponent<CShape>()) {
    if (auto raw = shape->getShape()) xf->applyTo(*raw);
  }
  if (auto tex = getComponent<Texture>()) {
    tex->setPosition(xf->getPosition());
    auto s = xf->getScale();
    if (s.x == 0.f && s.y == 0.f) s = { 1.f, 1.f }; // <- Fallback
    tex->setScale(s);
    tex->setRotation(xf->getRotation()); // en Texture.cpp usa sf::degrees(...)
  }
}

void Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {
  // Solo el Track dibuja la shape (y nada más)
  if (m_name == "Track") {
    if (auto shape = getComponent<CShape>()) {
      shape->render(window);
    }
    return; // <- evita dibujar el sprite del Track (segunda capa de mapa)
  }

  // Los demás actorean (racers) dibujan su sprite
  if (auto textureComp = getComponent<Texture>()) {
    textureComp->render(window);
  }
}

void Actor::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  if (texture.isNull()) return;

  // Reemplaza o agrega el componente Texture del actor
  bool replaced = false;
  for (auto& comp : components) {
    if (comp.template dynamic_pointer_cast<Texture>()) {
      comp = texture;
      replaced = true;
      break;
    }
  }
  if (!replaced) components.push_back(texture);

  // Solo el actor "Track" recibe la textura en su CShape
  if (m_name == "Track") {
    if (auto shape = getComponent<CShape>()) {
      shape->setTexture(texture);
    }
  }
}
