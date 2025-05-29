#pragma once
#include "Prerequisites.h"

class Window;

class 
CShape {
public:
	CShape() default;


	CShape(ShapeType shapeType) : m_shape(nullptr), m_shapeType(ShapeType::EMPTY) { }


	~CShape() default;

	sf::Shape* 
    createShape(ShapeType shapeType);

	void
	update(float deltaTime);

	void
	render(Window& window);

	void
	setPosition(float x, float y);

	void
	setPosition(const sf::Vector2f& position);

	void
	setFillColor(const sf::Color& color);

	void
	setRotation(float angle);

	void
	setScale(const sf::Vector2f& sccl);

	sf::Shape*
	getShape() {
		return m_shape;
	}

private:
  sf::Shape* m_shape;
  ShapeType m_shapeType;
  sf::VertexArray* m_line;
};