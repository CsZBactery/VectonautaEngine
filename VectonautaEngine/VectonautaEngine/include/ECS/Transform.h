#pragma once

#include "Prerequisites.h"
#include "ECS/Component.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>        // para sf::degrees
#include <SFML/Graphics/Transformable.hpp> // para applyTo
#include <cmath>

namespace EngineUtilities { template<typename T> class TSharedPointer; }
class Window;

class Transform : public Component {
public:
  Transform()
    : Component(ComponentType::TRANSFORM)
    , m_position(0.f, 0.f)
    , m_rotationDegrees(0.f)
    , m_scale(1.f, 1.f)
  {
  }

  ~Transform() override = default;

  void start() override {}
  void update(float /*deltaTime*/) override {}
  void render(const EngineUtilities::TSharedPointer<Window>& /*window*/) override {}
  void destroy() override {}

  void seek(const sf::Vector2f& target, float maxSpeed, float deltaTime, float arriveRadius = 10.f) {
    sf::Vector2f toTarget = target - m_position;
    float dist = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
    if (dist < 0.001f) return;

    sf::Vector2f direction = { toTarget.x / dist, toTarget.y / dist };
    float speed = maxSpeed;
    if (dist < arriveRadius) {
      speed = maxSpeed * (dist / arriveRadius);
    }
    m_position += direction * speed * deltaTime;
  }

  // Setters
  void setPosition(const sf::Vector2f& pos) { m_position = pos; }
  void setRotation(float degrees) { m_rotationDegrees = degrees; }
  void setScale(const sf::Vector2f& scale) { m_scale = scale; }

  // Getters
  const sf::Vector2f& getPosition() const { return m_position; }
  float getRotation() const { return m_rotationDegrees; }
  const sf::Vector2f& getScale() const { return m_scale; }

  // Aplicar a un sf::Transformable (shape / sprite)
  void applyTo(sf::Transformable& t) const {
    t.setPosition(m_position);
    t.setRotation(sf::degrees(m_rotationDegrees));
    t.setScale(m_scale);
  }

private:
  sf::Vector2f m_position;
  float        m_rotationDegrees; // en grados
  sf::Vector2f m_scale;
};
