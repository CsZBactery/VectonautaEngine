#pragma once

/**
 * @file Transform.h
 * @brief Component that manages position, rotation, and scale of an entity.
 */

#include "Prerequisites.h"
#include "ECS/Component.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transformable.hpp> // para sf::Transformable
#include <SFML/System/Angle.hpp>           // para sf::degrees
#include <cmath>

namespace EngineUtilities { template<typename T> class TSharedPointer; }
class Window;

/**
 * @class Transform
 * @brief Component for 2D transform: position, rotation (en grados), and scale.
 */
class Transform : public Component {
public:
  Transform()
    : Component(ComponentType::TRANSFORM)
    , m_position(0.f, 0.f)
    , m_rotation(0.f) // en grados
    , m_scale(1.f, 1.f)
  {
  }

  ~Transform() override = default;

  void start() override {}
  void update(float /*deltaTime*/) override {}
  void render(const EngineUtilities::TSharedPointer<Window>& /*window*/) override {}
  void destroy() override {}

  /**
   * @brief Seek hacia target con velocidad máxima, desacelerando al acercarse dentro de arriveRadius.
   * @param target Punto objetivo.
   * @param maxSpeed Velocidad máxima en unidades por segundo.
   * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
   * @param arriveRadius Radio dentro del cual desacelera al acercarse.
   */
  void seek(const sf::Vector2f& target, float maxSpeed, float deltaTime, float arriveRadius = 10.f) {
    sf::Vector2f toTarget = target - m_position;
    float dist = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
    if (dist < 0.001f) return;

    sf::Vector2f direction = { toTarget.x / dist, toTarget.y / dist };

    // Ajustar velocidad para desacelerar al acercarse
    float speed = maxSpeed;
    if (dist < arriveRadius) {
      speed = maxSpeed * (dist / arriveRadius);
    }

    m_position += direction * speed * deltaTime;
  }

  // Setters
  void setPosition(const sf::Vector2f& pos) { m_position = pos; }
  void setRotation(float degrees) { m_rotation = degrees; } // se guarda en grados
  void setScale(const sf::Vector2f& scale) { m_scale = scale; }

  // Getters
  const sf::Vector2f& getPosition() const { return m_position; }
  float getRotation() const { return m_rotation; } // devuelve grados
  const sf::Vector2f& getScale() const { return m_scale; }

  /**
   * @brief Aplica este transform a un objeto sf::Transformable (shape, sprite, etc.).
   * Convierte la rotación de grados a sf::Angle adecuadamente.
   */
  void applyTo(sf::Transformable& t) const {
    t.setPosition(m_position);
    t.setRotation(sf::degrees(m_rotation));
    t.setScale(m_scale);
  }

private:
  sf::Vector2f m_position;
  float        m_rotation; // en grados
  sf::Vector2f m_scale;
};
