#pragma once

/**
 * @file Transform.h
 * @brief Component that manages position, rotation, and scale of an entity.
 */

#include "Prerequisites.h"          // Core includes and SFML headers
#include "ECS/Component.h"          // Base Component class
#include <SFML/System/Vector2.hpp>   // sf::Vector2f
#include <cmath>                     // std::sqrt

 // Forward declarations
namespace EngineUtilities { template<typename T> class TSharedPointer; }
class Window;

/**
 * @class Transform
 * @brief Component for 2D transform: position, rotation, and scale.
 */
class Transform : public Component {
public:
  /**
   * @brief Constructs a Transform with default values.
   */
  Transform()
    : Component(ComponentType::TRANSFORM)
    , m_position(0.f, 0.f)
    , m_rotation(0.f, 0.f)
    , m_scale(1.f, 1.f)
  {
  }

  /**
   * @brief Default destructor.
   */
  ~Transform() override = default;

  void start() override {}
  void update(float /*deltaTime*/) override {}
  void render(const EngineUtilities::TSharedPointer<Window>& /*window*/) override {}
  void destroy() override {}

  /**
   * @brief Moves towards a target position at a given speed, stopping within a range.
   */
  void seek(const sf::Vector2f& target, float speed, float deltaTime, float range) {
    sf::Vector2f dir = target - m_position;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length > range) {
      dir /= length;
      m_position += dir * speed * deltaTime;
    }
  }

  // Setters
  void setPosition(const sf::Vector2f& pos) { m_position = pos; }
  void setRotation(const sf::Vector2f& rot) { m_rotation = rot; }
  void setScale(const sf::Vector2f& scale) { m_scale = scale; }

  // Getters
  const sf::Vector2f& getPosition() const { return m_position; }
  const sf::Vector2f& getRotation() const { return m_rotation; }
  const sf::Vector2f& getScale()    const { return m_scale; }

private:
  sf::Vector2f m_position;
  sf::Vector2f m_rotation;
  sf::Vector2f m_scale;
};
