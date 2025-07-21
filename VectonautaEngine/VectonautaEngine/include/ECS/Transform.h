#pragma once
#include "Prerequisites.h"
#include "ECS/Component.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>
/**
 * @file Transform.h
 * @brief Declares the Transform component used for managing position, rotation, and scale in entities.
 */

class Window;
/**
 * @class Transform
 * @brief Component that holds position, rotation, and scale for an entity.
 */

class Transform : public Component {
public:
  /**
   * @brief Default constructor.
   */
  Transform()
    : Component(ComponentType::TRANSFORM),
    m_position(0.f, 0.f),
    m_rotation(0.f, 0.f),
    m_scale(1.f, 1.f) {
  }

  /**
   * @brief Destructor.
   */
  virtual
    ~Transform() = default;

  // Métodos override

  //Quitar deltaTime 
  void
    start() override {}


  void
    update(float deltaTime) override {}


  void
    render(const EngineUtilities::TSharedPointer<Window>& window) override {}

  void
    destroy() override {
  }

  void
    seek(const sf::Vector2f& targetPosition,
      float speed,
      float deltaTime,
      float range) {
    sf::Vector2f direction = targetPosition - m_position;
    float lenght = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (lenght > range) {
      direction /= lenght;
      m_position += direction * speed * deltaTime;
    }
  }

  // Setters
  void
    setPosition(const sf::Vector2f& _position) {
    m_position = _position;
  }   //Actualiza el valor de la clase 

  void
    setRotation(const sf::Vector2f& _rotation) {
    m_rotation = _rotation;
  }

  void
    setScale(const sf::Vector2f& _scale) {
    m_scale = _scale;
  }

  // Getters
  sf::Vector2f
    getPosition() const { return m_position; }

  sf::Vector2f
    getRotation() const { return m_rotation; }

  sf::Vector2f
    getScale()    const { return m_scale; }

private:
  sf::Vector2f m_position;
  sf::Vector2f m_rotation;
  sf::Vector2f m_scale;
};