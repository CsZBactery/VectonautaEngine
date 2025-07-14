#include "CShape.h"
#include "Window.h"

/**
 * @file CShape.cpp
 * @brief Implementation of the CShape class for creating and manipulating different SFML shapes.
 */

 // Constructor por defecto
CShape::CShape()
  : Component(ComponentType::SHAPE),
  m_shapePtr(nullptr),
  m_shapeType(ShapeType::EMPTY) {
}

// Constructor con tipo de forma
CShape::CShape(ShapeType shapeType)
  : Component(ComponentType::SHAPE),
  m_shapePtr(nullptr),
  m_shapeType(ShapeType::EMPTY) {
  createShape(shapeType);
}

// Método que crea la figura
void CShape::createShape(ShapeType type) {
  m_shapeType = type;

  switch (type) {
  case ShapeType::CIRCLE: {
    auto circleSP = EngineUtilities::MakeShared<sf::CircleShape>(10.f);
    circleSP->setFillColor(sf::Color::White);
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
    triangleSP->setFillColor(sf::Color::White);
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
    polygonSP->setFillColor(sf::Color::White);
    m_shapePtr = polygonSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  default:
    m_shapePtr.reset();
    ERROR("CShape", "createShape", "Unknown shape type");
    return;
  }
}

void CShape::start() {
  // Lógica inicial (por ahora vacía)
}

void CShape::update(float deltaTime) {
  // Animación o lógica futura
}

void CShape::render(const EngineUtilities::TSharedPointer<Window>& window) {
  if (m_shapePtr) {
    window->draw(*m_shapePtr);
  }
}

void CShape::destroy() {
  // No se requiere lógica adicional por ahora
}

void CShape::setPosition(float x, float y) {
  if (m_shapePtr)
    m_shapePtr->setPosition(x, y);
  else
    ERROR("CShape", "setPosition", "Shape is not initialized.");
}

void CShape::setPosition(const sf::Vector2f& position) {
  if (m_shapePtr)
    m_shapePtr->setPosition(position);
  else
    ERROR("CShape", "setPosition", "Shape is not initialized.");
}

void CShape::setFillColor(const sf::Color& color) {
  if (m_shapePtr)
    m_shapePtr->setFillColor(color);
  else
    ERROR("CShape", "setFillColor", "Shape is not initialized.");
}

void CShape::SetRotation(float angle) {
  if (m_shapePtr)
    m_shapePtr->setRotation(angle);
  else
    ERROR("CShape", "SetRotation", "Shape is not initialized.");
}

void CShape::setScale(const sf::Vector2f& scale) {
  if (m_shapePtr)
    m_shapePtr->setScale(scale);
  else
    ERROR("CShape", "setScale", "Shape is not initialized.");
}
