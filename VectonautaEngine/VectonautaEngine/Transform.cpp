#include "Prerequisites.h"
#include "ECS/Transform.h"
#include <cmath>


void Transform::seek(const sf::Vector2f& target,
  float maxSpeed,
  float deltaTime,
  float arrivalRadius)
{
  sf::Vector2f toTarget = target - m_position;
  float dist = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
  if (dist > 0.f) {
    sf::Vector2f desired = toTarget / dist * maxSpeed;
    m_position += desired * deltaTime;
  }
}