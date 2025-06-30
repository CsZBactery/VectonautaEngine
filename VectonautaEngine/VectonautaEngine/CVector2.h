#pragma once

#include "../Prerequisites.h"
#include <SFML/System/Vector2.hpp>
#include <iostream>

class CVector2 {
public:
  float x;
  float y;

  // Para los constructores
  CVector2() : x(0.f), y(0.f) {}
  CVector2(float x, float y) : x(x), y(y) {}

  // Para los operadores aritmeticos
  CVector2 operator+(const CVector2& other) const {
    return CVector2(x + other.x, y + other.y);
  }

  CVector2 operator-(const CVector2& other) const {
    return CVector2(x - other.x, y - other.y);
  }

  CVector2 operator*(float scalar) const {
    return CVector2(x * scalar, y * scalar);
  }

  CVector2 operator/(float scalar) const {
    return CVector2(x / scalar, y / scalar);
  }

  
  CVector2& operator+=(const CVector2& other) {
    x += other.x; y += other.y;
    return *this;
  }

  CVector2& operator-=(const CVector2& other) {
    x -= other.x; y -= other.y;
    return *this;
  }

  CVector2& operator*=(float scalar) {
    x *= scalar; y *= scalar;
    return *this;
  }

  CVector2& operator/=(float scalar) {
    x /= scalar; y /= scalar;
    return *this;
  }

  
  bool operator==(const CVector2& other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const CVector2& other) const {
    return !(*this == other);
  }

  
  float& operator[](int index) {
    return (index == 0) ? x : y;
  }

  const float& operator[](int index) const {
    return (index == 0) ? x : y;
  }

  
  friend std::ostream& operator<<(std::ostream& os, const CVector2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
  }

  
  float dot(const CVector2& other) const {
    return x * other.x + y * other.y;
  }

  
  float cross(const CVector2& other) const {
    return x * other.y - y * other.x;
  }

  
  static CVector2 zero() {
    return CVector2(0.f, 0.f);
  }

  static CVector2 one() {
    return CVector2(1.f, 1.f);
  }

  
  void move(const sf::Vector2f& offset) {
    x += offset.x;
    y += offset.y;
  }

 
  float lengthSquared() const {
    return x * x + y * y;
  }
};
