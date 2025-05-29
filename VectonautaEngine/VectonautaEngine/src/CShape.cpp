#include "CShape.h"
#include "Window.h"

sf::Shape* 
CShape::createShape(ShapeType shapeType) {
	m_shapeType = shapeType;
	switch (shapeType) {
	case ShapeType::CIRCLE:
	{
		sf::CircleShape* circle = new sf::CircleShape(10.f); //
		circle->setFillColor(sf::Color::Magenta);
		return circle;
	}
	case ShapeType::RECTANGLE:
	{
		sf::RectangleShape* rectangle = new sf::RectangleShape(sf::Vector2f(100.f, 50.f));
		rectangle->setFillColor(sf::Color::Red);
		return rectangle;
	}
	case ShapeType::TRIANGLE:
	{
		sf::ConvexShape* triangle = new sf::ConvexShape(3);
		triangle->setPoint(0, sf::Vector2f(0.f, 0.f));
		triangle->setPoint(1, sf::Vector2f(50.f, 100.f));
		triangle->setPoint(2, sf::Vector2f(100.f, 0.f));
		triangle->setFillColor(sf::Color::Yellow);
		m_shape = triangle;
		return triangle;
	}
	case ShapeType::POLYGON:
	{
		sf::ConvexShape* polygon = new sf::ConvexShape(5);
		polygon->setPoint(0, sf::Vector2f(0.f, 0.f));
		polygon->setPoint(1, sf::Vector2f(50.f, 100.f));
		polygon->setPoint(2, sf::Vector2f(100.f, 0.f));
		polygon->setPoint(3, sf::Vector2f(75.f, -50.f));
		polygon->setPoint(4, sf::Vector2f(-25.f, -50.f));
		polygon->setFillColor(sf::Color::White);
		m_shape = polygon;
		return polygon;
	}
          default:
			  break;
	}
	return nullptr;
}

void 
CShape::setPosition(float x, float y) {
	if (m_shape) {
		m_shape->setPosition(x, y);
	}
	else {
		ERROR("CShape", "setPosition", "Shape is not initialized.");
	}
}

void 
CShape::setPosition(const sf::Vector2f& position) {
	if (m_shape) {
		m_shape->setPosition(position);
	}
	else {
		ERROR("CShape", "setPosition", "Shape is not initialized.");
	}
}

void 
CShape::setFillColor(const sf::Color& color) {
	if (m_shape) {
		m_shape->setFillColor(color);
	}
	else {
		ERROR("CShape", "setFillColor", "Shape is not initialized.");
	}
}

void 
CShape::setRotation(float angle) {
	if (m_shape) {
		m_shape->setRotation(angle);
	}
	else {
		ERROR("CShape", "setRotation", "Shape is not initialized.");
	}
}

void 
CShape::setScale(const sf::Vector2f& scl) {
	if (m_shape) {
		m_shape->setScale(scl);
	}
	else {
		ERROR("CShape", "setScale", "Shape is not initialized.");
	}
}
