﻿#pragma once

/**
 * @file CShape.h
 * @brief Declares the CShape class used to represent 2D shapes as components in the ECS system.
 */

#include "Prerequisites.h"
#include "Memory/TSharedPointer.h"
#include "Memory/TUniquePtr.h"
#include "ECS/Component.h"

  class Window;

/**
 * @class CShape
 * @brief A component that represents a drawable 2D shape using SFML.
 *
 * Supports circle, rectangle, triangle, and polygon shapes.
 */
class CShape : public Component {
public:
  /**
   * @brief Default constructor.
   */
  CShape() = default;

  /**
   * @brief Constructs a CShape component with a specific shape type.
   * @param shapeType Type of the shape to initialize.
   */
  CShape(ShapeType shapeType);

  /**
   * @brief Destructor.
   */
  virtual
    ~CShape() = default;

  /**
   * @brief Initializes the component.
   */
  void
    start() override;

  /**
   * @brief Updates the shape logic.
   * @param deltaTime Time elapsed since last frame.
   */
  void
    update(float deltaTime) override;

  /**
   * @brief Renders the shape on screen.
   * @param window Pointer to the rendering window.
   */
  void
    render(const EngineUtilities::TSharedPointer<Window>& window) override;

  /**
   * @brief Cleans up the component resources.
   */
  void
    destroy() override;

  /**
   * @brief Creates a new shape based on the specified type.
   * @param shapeType Type of shape to create.
   */
  void
    createShape(ShapeType shapeType);

  /**
   * @brief Sets the shape position using coordinates.
   * @param x X coordinate.
   * @param y Y coordinate.
   */
  void
    setPosition(float x, float y);

  /**
   * @brief Sets the shape position using a vector.
   * @param position SFML 2D vector.
   */
  void
    setPosition(const sf::Vector2f& position);

  /**
   * @brief Sets the fill color of the shape.
   * @param color Color to apply.
   */
  void
    setFillColor(const sf::Color& color);

  /**
   * @brief Sets the shape's rotation.
   * @param angle Angle in degrees.
   */
  void
    setRotation(float angle);

  /**
   * @brief Sets the scale of the shape.
   * @param scl Scale factor as a 2D vector.
   */
  void
    setScale(const sf::Vector2f& scl);

  /**
   * @brief Returns the raw shape pointer (for low-level access).
   * @return Pointer to the internal SFML shape, or nullptr if not set.
   */
  sf::Shape* getShape();

private:
  EngineUtilities::TSharedPointer<sf::Shape> m_shapePtr; ///< Smart pointer to the SFML shape.
  ShapeType m_shapeType = ShapeType::EMPTY;              ///< Type of the current shape.
  sf::VertexArray* m_line = nullptr;                     ///< Reserved for line shapes (optional).
};