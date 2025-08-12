#include "CShape.h"
#include "Window.h"

CShape::CShape()
  : Component(ComponentType::SHAPE)
  , m_shapeType(ShapeType::EMPTY)
{
  // Evita puntero nulo creando una forma por defecto.
  // Elegimos CIRCLE como “placeholder”.
  createShape(ShapeType::CIRCLE);
}

CShape::CShape(ShapeType shapeType)
  : Component(ComponentType::SHAPE)
  , m_shapeType(ShapeType::EMPTY)
{
  // Crea directamente la forma solicitada.
  createShape(shapeType);
}

void CShape::createShape(ShapeType shapeType) {
  m_shapeType = shapeType;

  // Según el tipo, instanciamos la forma SFML correspondiente.
  switch (shapeType) {
  case ShapeType::CIRCLE: {
    // Círculo con radio 10 px, color verde por defecto.
    auto circleSP = EngineUtilities::MakeShared<sf::CircleShape>(10.f);
    circleSP->setFillColor(sf::Color::Green);
    m_shapePtr = circleSP;
    break;
  }
  case ShapeType::RECTANGLE: {
    // Rectángulo 100x50 px, color blanco por defecto.
    auto rectSP = EngineUtilities::MakeShared<sf::RectangleShape>(sf::Vector2f(100.f, 50.f));
    rectSP->setFillColor(sf::Color::White);
    m_shapePtr = rectSP;
    break;
  }
  case ShapeType::TRIANGLE: {
    // Triángulo básico usando ConvexShape de 3 puntos.
    auto convex = EngineUtilities::MakeShared<sf::ConvexShape>(3);
    convex->setPoint(0, sf::Vector2f(0.f, 0.f));
    convex->setPoint(1, sf::Vector2f(50.f, 100.f));
    convex->setPoint(2, sf::Vector2f(100.f, 0.f));
    convex->setFillColor(sf::Color::Blue);
    m_shapePtr = convex;
    break;
  }
  case ShapeType::POLYGON: {
    // Polígono ejemplo de 5 puntos (forma arbitraria).
    auto poly = EngineUtilities::MakeShared<sf::ConvexShape>(5);
    poly->setPoint(0, sf::Vector2f(0.f, 0.f));
    poly->setPoint(1, sf::Vector2f(50.f, 100.f));
    poly->setPoint(2, sf::Vector2f(100.f, 0.f));
    poly->setPoint(3, sf::Vector2f(75.f, -50.f));
    poly->setPoint(4, sf::Vector2f(-25.f, -50.f));
    poly->setFillColor(sf::Color::Red);
    m_shapePtr = poly;
    break;
  }
  default:
    // Tipo no soportado: resetea y avisa.
    m_shapePtr.reset();
    ERROR("CShape", "createShape", "Unknown shape type");
    return;
  }
}

// Ciclo de vida ECS: en este componente no hacemos nada especial.
void CShape::start() {}
void CShape::update(float) {}
void CShape::destroy() {}

void CShape::render(const EngineUtilities::TSharedPointer<Window>& window) {
  // Si hay forma válida, dibuja. Si no, loguea error.
  if (m_shapePtr) {
    window->draw(*m_shapePtr);
  }
  else {
    ERROR("CShape", "render", "Shape is not initialized.");
  }
}

void CShape::setPosition(float x, float y) {
  // Overload práctico: crea vector y delega en SFML.
  if (m_shapePtr) {
    m_shapePtr->setPosition(sf::Vector2f(x, y));
  }
  else {
    ERROR("CShape", "setPosition", "Shape is not initialized.");
  }
}

void CShape::setPosition(const sf::Vector2f& position) {
  // Asigna posición absoluta en coordenadas de mundo.
  if (m_shapePtr) {
    m_shapePtr->setPosition(position);
  }
  else {
    ERROR("CShape", "setPosition", "Shape is not initialized.");
  }
}

void CShape::setFillColor(const sf::Color& color) {
  // Cambia el color de relleno de la forma.
  if (m_shapePtr) {
    m_shapePtr->setFillColor(color);
  }
  else {
    ERROR("CShape", "setFillColor", "Shape is not initialized.");
  }
}

void CShape::setRotation(float angleDegrees) {
  // SFML 3 usa sf::Angle: convierte grados con sf::degrees(...)
  if (m_shapePtr) {
    m_shapePtr->setRotation(sf::degrees(angleDegrees));
  }
  else {
    ERROR("CShape", "setRotation", "Shape is not initialized.");
  }
}

void CShape::setScale(const sf::Vector2f& scl) {
  // Escala no uniforme por eje (x,y).
  if (m_shapePtr) {
    m_shapePtr->setScale(scl);
  }
  else {
    ERROR("CShape", "setScale", "Shape is not initialized.");
  }
}

sf::Shape* CShape::getShape() {
  // Devuelve el puntero crudo (puede ser nullptr si no se creó).
  return m_shapePtr.get();
}

void CShape::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  // Asigna la textura subyacente al shape (si existe).
  // Ojo: el shape almacena un puntero a sf::Texture; la vida útil de 'texture' debe cubrir el render.
  if (m_shapePtr && texture && !texture.isNull()) {
    m_shapePtr->setTexture(&texture->getTexture());
  }
}
