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
 * @brief A component that represents a 2D SFML shape and can hold a texture.
 */
class CShape : public Component {
public:
  /**
   * @brief Default constructor. Shape is initially empty (no geometry).
   */
  CShape();

  /**
   * @brief Constructs and immediately creates a shape of the given type.
   * @param shapeType Desired primitive type to instantiate.
   */
  explicit CShape(ShapeType shapeType);

  /**
   * @brief Virtual destructor.
   */
  ~CShape() override = default;

  /**
   * @brief Called once when the component starts.
   */
  void start() override;

  /**
   * @brief Per-frame update for the component.
   * @param deltaTime Time elapsed since last frame in seconds.
   */
  void update(float deltaTime) override;

  /**
   * @brief Renders the shape to the given window.
   * @param window Target window wrapper to draw into.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  /**
   * @brief Releases resources owned by this component.
   */
  void destroy() override;

  /**
   * @brief Creates or replaces the underlying SFML shape with a new type.
   * @param shapeType Primitive to build (RECTANGLE, CIRCLE, etc.).
   */
  void createShape(ShapeType shapeType);

  /**
   * @brief Sets the world position of the shape.
   * @param x X coordinate.
   * @param y Y coordinate.
   */
  void setPosition(float x, float y);

  /**
   * @brief Sets the world position of the shape.
   * @param position Position vector.
   */
  void setPosition(const sf::Vector2f& position);

  /**
   * @brief Changes the fill color of the shape.
   * @param color New color to apply.
   */
  void setFillColor(const sf::Color& color);

  /**
   * @brief Sets the rotation of the shape in degrees.
   * @param angleDegrees Angle in degrees (clockwise).
   */
  void setRotation(float angleDegrees);

  /**
   * @brief Sets the scale of the shape on each axis.
   * @param scl Scale factors (x,y).
   */
  void setScale(const sf::Vector2f& scl);

  /**
   * @brief Returns the raw SFML shape pointer (may be nullptr if not created).
   * @return Pointer to the underlying sf::Shape.
   */
  sf::Shape* getShape();

  /**
   * @brief Assigns a texture component to be used as fill for the shape.
   * @param texture Texture component (wrapping an sf::Texture).
   */
  void setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

private:
  /**
   * @brief Owned pointer to the underlying SFML shape instance.
   */
  EngineUtilities::TSharedPointer<sf::Shape> m_shapePtr;

  /**
   * @brief Current primitive type represented by this component.
   */
  ShapeType m_shapeType = ShapeType::EMPTY;
};
