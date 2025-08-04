#include "Prerequisites.h"
#include "CShape.h"
#include "Window.h"
#include "Memory/TUniquePtr.h"
#include "Memory/TSharedPointer.h"
#include <ECS/Texture.h>
#include <SFML/Graphics.hpp> // por si no está transitivamente incluido

/**
 * @file CShape.cpp
 * @brief Implementation of the CShape class for creating and manipulating different SFML shapes.
 */

void CShape::createShape(ShapeType shapeType) {
  m_shapeType = shapeType;

  switch (shapeType) {
  case ShapeType::CIRCLE: {
    auto circleSP = EngineUtilities::MakeShared<sf::CircleShape>(10.f);
    circleSP->setFillColor(sf::Color::Green);
    circleSP->setOrigin({ 10.f, 10.f }); // centrar el origen si se desea
    m_shapePtr = circleSP;
    break;
  }
  case ShapeType::RECTANGLE: {
    auto rectangleSP = EngineUtilities::MakeShared<sf::RectangleShape>(sf::Vector2f(100.f, 50.f));
    rectangleSP->setFillColor(sf::Color::White);
    rectangleSP->setOrigin({ 50.f, 25.f });
    m_shapePtr = rectangleSP;
    break;
  }
  case ShapeType::TRIANGLE: {
    auto triangleSP = EngineUtilities::MakeShared<sf::ConvexShape>(3);
    triangleSP->setPoint(0, sf::Vector2f(0.f, 0.f));
    triangleSP->setPoint(1, sf::Vector2f(50.f, 100.f));
    triangleSP->setPoint(2, sf::Vector2f(100.f, 0.f));
    triangleSP->setFillColor(sf::Color::Blue);
    // opcional: centrar origen si necesitas rotar alrededor de centro
    triangleSP->setOrigin({ 50.f, 50.f });
    m_shapePtr = triangleSP;
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
    polygonSP->setOrigin({ 50.f, 25.f }); // ejemplo
    m_shapePtr = polygonSP;
    break;
  }
  default:
    if (m_shapePtr) {
      m_shapePtr.reset(); // asumir que tiene reset semántico
    }
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

void CShape::start() {
  // Inicialización si hace falta
}

void CShape::update(float deltaTime) {
  // Lógica futura
}

void CShape::destroy() {
  // Limpieza si aplica (por ejemplo, soltar recursos si no lo hace el smart pointer)
}

void CShape::render(const EngineUtilities::TSharedPointer<Window>& window) {
  if (!window) {
    ERROR("CShape", "render", "Window inválida.");
    return;
  }

  if (m_shapePtr) {
    // Se asume que Window expone draw(a) de forma compatible con sf::Drawable
    window->draw(*m_shapePtr);
  }
  else {
    ERROR("CShape", "render", "Shape is not initialized.");
  }
}

void CShape::setPosition(float x, float y) {
  if (m_shapePtr) {
    m_shapePtr->setPosition({ x, y });
  }
  else {
    ERROR("CShape", "setPosition", "Shape is not initialized.");
  }
}

void CShape::setPosition(const sf::Vector2f& position) {
  if (m_shapePtr) {
    m_shapePtr->setPosition(position);
  }
  else {
    ERROR("CShape", "setPosition", "Shape is not initialized.");
  }
}

void CShape::setFillColor(const sf::Color& color) {
  if (m_shapePtr) {
    m_shapePtr->setFillColor(color);
  }
  else {
    ERROR("CShape", "setFillColor", "Shape is not initialized.");
  }
}

void CShape::setRotation(float angle) {
  if (m_shapePtr) {
    m_shapePtr->setRotation(sf::degrees(angle)); // SFML 3 requiere sf::Angle
  }
  else {
    ERROR("CShape", "setRotation", "Shape is not initialized.");
  }
}

void CShape::setScale(const sf::Vector2f& scale) {
  if (m_shapePtr) {
    m_shapePtr->setScale(scale);
  }
  else {
    ERROR("CShape", "setScale", "Shape is not initialized.");
  }
}

sf::Shape* CShape::getShape() {
  return m_shapePtr ? m_shapePtr.get() : nullptr;
}

void CShape::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  if (m_shapePtr && texture) {
    m_shapePtr->setTexture(&texture->getTexture());
  }
}
