#pragma once

/**
 * @file CShape.h
 * @brief Declares the CShape class used to represent 2D shapes as components in the ECS system.
 */

#include <SFML/Graphics.hpp> // sf::Shape, sf::CircleShape, etc.
#include <Prerequisites.h>   // asume que ahí está ShapeType y ComponentType
#include <Memory/TSharedPointer.h>
#include <ECS/Component.h>
#include <ECS/Texture.h>

class Window;

/**
 * @class CShape
 * @brief A component que representa una forma 2D con SFML y puede tener textura.
 */
class CShape : public Component {
public:
  CShape();
  explicit CShape(ShapeType shapeType);
  ~CShape() override = default;

  void start() override;
  void update(float deltaTime) override;
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;
  void destroy() override;

  void createShape(ShapeType shapeType);

  void setPosition(float x, float y);
  void setPosition(const sf::Vector2f& position);
  void setFillColor(const sf::Color& color);
  void setRotation(float angleDegrees);
  void setScale(const sf::Vector2f& scl);

  sf::Shape* getShape();
  void setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

private:
  EngineUtilities::TSharedPointer<sf::Shape> m_shapePtr;
  ShapeType m_shapeType = ShapeType::EMPTY;
};
