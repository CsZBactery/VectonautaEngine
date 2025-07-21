#include "CShape.h"
#include "Window.h"
#include "Memory/TUniquePtr.h"
#include <Memory/TSharedPointer.h>
/**
 * @file CShape.cpp
 * @brief Implementation of the CShape class for creating and manipulating different SFML shapes.
 */

 /**
  * @brief Creates a shape of the specified type.
  *
  * Allocates and configures a shape (Circle, Rectangle, Triangle, or Polygon) based on the given shape type.
  * The shape is stored internally using a shared pointer.
  *
  * @param shapeType The type of shape to create.
  */
void
CShape::createShape(ShapeType shapeType) {
  m_shapeType = shapeType;

  switch (shapeType) {
  case ShapeType::CIRCLE: {
    auto circleSP = EngineUtilities::MakeShared<sf::CircleShape>(10.f);
    circleSP->setFillColor(sf::Color::Green);
    m_shapePtr = circleSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  case ShapeType::RECTANGLE: {
    auto rectangleSP = EngineUtilities::MakeShared<sf::RectangleShape>(sf::Vector2f(100.f, 50.f));
    rectangleSP->setFillColor(sf::Color::White);
    m_shapePtr = rectangleSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  case ShapeType::TRIANGLE: {
    auto triangleSP = EngineUtilities::MakeShared<sf::ConvexShape>(3);
    triangleSP->setPoint(0, sf::Vector2f(0.f, 0.f));
    triangleSP->setPoint(1, sf::Vector2f(50.f, 100.f));
    triangleSP->setPoint(2, sf::Vector2f(100.f, 0.f));
    triangleSP->setFillColor(sf::Color::Blue);
    m_shapePtr = triangleSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  case ShapeType::POLYGON: {
    auto polygonSP = EngineUtilities::MakeShared<sf::ConvexShape>(5);
    polygonSP->setPoint(0, sf::Vector2f(0.f, 0.f));
    polygonSP->setPoint(1, sf::Vector2f(50.f, 100.f));
    polygonSP->setPoint(2, sf::Vector2f(100.f, 0.f));
    polygonSP->setPoint(3, sf::Vector2f(75.f, -50.f));
    polygonSP->setPoint(4, sf::Vector2f(-25.f, -50.f));
    polygonSP->setFillColor(sf::Color::Red);
    m_shapePtr = polygonSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  default:
    m_shapePtr.reset();
    ERROR("CShape", "createShape", "Unknown shape type");
    return;
  }
}

CShape::CShape()
  : Component(ComponentType::SHAPE),
  m_shapePtr(nullptr),
  m_shapeType(ShapeType::EMPTY) {
}

CShape::CShape(ShapeType shapeType)
  : Component(ComponentType::SHAPE),
  m_shapePtr(nullptr),
  m_shapeType(ShapeType::EMPTY) {
  createShape(shapeType);
}


void
CShape::start() {
  // Tu lógica aquí
}

void
CShape::update(float deltaTime) {
  // Future logic for animation or state change
}

void
CShape::destroy() {

}


/**
 * @brief Renders the shape using the given window.
 *
 * @param window Shared pointer to the window object.
 */
void
CShape::render(const EngineUtilities::TSharedPointer<Window>& window) {
  if (m_shapePtr) {
    window->draw(*m_shapePtr);
  }
  else {
    ERROR("CShape", "render", "Shape is not initialized.");
  }
}

/**
 * @brief Sets the position of the shape.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void
CShape::setPosition(float x, float y) {
  if (m_shapePtr) {
    m_shapePtr->setPosition(x, y);
  }
  else {
    ERROR("CShape", "setPosition", "Shape is not initialized.");
  }
}

/**
 * @brief Sets the position of the shape using a vector.
 *
 * @param position The position as a 2D vector.
 */
void
CShape::setPosition(const sf::Vector2f& position) {
  if (m_shapePtr) {
    m_shapePtr->setPosition(position);
  }
  else {
    ERROR("CShape", "setPosition", "Shape is not initialized.");
  }
}

/**
 * @brief Sets the fill color of the shape.
 *
 * @param color The color to apply.
 */
void
CShape::setFillColor(const sf::Color& color) {
  if (m_shapePtr) {
    m_shapePtr->setFillColor(color);
  }
  else {
    ERROR("CShape", "setFillColor", "Shape is not initialized.");
  }
}

/**
 * @brief Sets the rotation angle of the shape.
 *
 * @param angle The rotation angle in degrees.
 */
void
CShape::setRotation(float angle)
{
  if (m_shapePtr) {
    m_shapePtr->setRotation(angle);
  }
  else {
    ERROR("CShape", "setRotation", "Shape is not initialized.");
  }
}

/**
 * @brief Sets the scale of the shape.
 *
 * @param scale The scaling factor as a 2D vector.
 */
void
CShape::setScale(const sf::Vector2f& scale) {
  if (m_shapePtr) {
    m_shapePtr->setScale(scale);
  }
  else {
    ERROR("CShape", "setScale", "Shape is not initialized.");
  }
}

sf::Shape* CShape::getShape()
{
  return nullptr;
}

void
Cshape::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  if (!texture.isNull()) {
    m_shapePtr->setTexture(&texture->getTexture());
  }
}