#pragma once

#include <Prerequisites.h>

/**
 * @file CVector2.h
 * @brief Represents a custom 2D vector with common mathematical operations.
 */
class CVector2 {
public:

  float x;
  float y;

  //Constructors

  /**
   * @brief Default constructor. Initializes the vector to (0, 0).
   */
  CVector2() : x(0.f), y(0.f) {}

  /**
   * @brief Parameterized constructor.
   * @param x The X component.
   * @param y The Y component.
   */
  CVector2(float x, float y) : x(x), y(y) {}

  // Arithmetic operators
  CVector2
    operator+(const CVector2& other) const {
    return CVector2(x + other.x, y + other.y);
  }

  CVector2
    operator-(const CVector2& other) const {
    return CVector2(x - other.x, y - other.y);
  }

  CVector2
    operator*(float scalar) const {
    return CVector2(x * scalar, y * scalar);
  }

  CVector2
    operator/(float divisor) const {
    return CVector2(x / divisor, y / divisor);
  }

  // Compound assignment operators 
  CVector2&
    operator+=(const CVector2& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  CVector2&
    operator-=(const CVector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  CVector2&
    operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  CVector2&
    operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  // Comparison operators 
  bool
    operator==(const CVector2& other) const {
    return x == other.x && y == other.y;
  }

  bool
    operator!=(const CVector2& other) const {
    return !(*this == other);
  }

  // Index access 

  /**
   * @brief Access vector components by index.
   * @param index 0 for x, 1 for y.
   * @return Reference to the component.
   */
  float&
    operator[](int index) {
    return index == 0 ? x : y;
  }

  const float&
    operator[](int index) const {
    return index == 0 ? x : y;
  }

  //  Geometric functions 

  /**
   * @brief Calculates the length (magnitude) of the vector.
   * @return The Euclidean length.
   */
  float
    length() const {
    return sqrt(x * x + y * y);
  }

  /**
   * @brief Calculates the squared length. Useful for comparisons (avoids sqrt).
   * @return The squared length.
   */
  float
    lengthSquared() const {
    return x * x + y * y;
  }

  /**
   * @brief Calculates the dot (scalar) product between this and another vector.
   * @param other The other vector.
   * @return The dot product.
   */
  float
    dot(const CVector2& other) const {
    return x * other.x + y * other.y;
  }

  /**
   * @brief Calculates the 2D cross product (scalar z).
   * @param other The other vector.
   * @return The scalar cross product.
   */
  float
    cross(const CVector2& other) const {
    return x * other.y - y * other.x;
  }

  /**
   * @brief Returns a normalized copy of this vector.
   * @return A unit vector, or (0,0) if length is zero.
   */
  CVector2
    normalized() const {
    float len = length();
    if (len == 0.f) return CVector2(0.f, 0.f);
    return CVector2(x / len, y / len);
  }

  /**
   * @brief Normalizes this vector in-place.
   */
  void
    normalize() {
    float len = length();
    if (len != 0.f) {
      x /= len;
      y /= len;
    }
  }

  // Static utility methods 

  /**
   * @brief Calculates the distance between two points.
   * @param a First point.
   * @param b Second point.
   * @return Distance.
   */
  static float
    distance(const CVector2& a, const CVector2& b) {
    return (a - b).length();
  }

  /**
   * @brief Performs linear interpolation between two vectors.
   * @param a Start vector.
   * @param b End vector.
   * @param t Interpolation factor in [0,1].
   * @return Interpolated vector.
   */
  static
    CVector2 lerp(const CVector2& a, const CVector2& b, float t) {
    if (t < 0.f) t = 0.f;
    if (t > 1.f) t = 1.f;
    return a + (b - a) * t;
  }

  /**
   * @brief Returns a zero vector (0,0).
   */
  static
    CVector2 zero() {
    return CVector2(0.f, 0.f);
  }

  /**
   * @brief Returns a vector with all components set to 1.
   */
  static
    CVector2 one() {
    return CVector2(1.f, 1.f);
  }

  // Transform-style debug methods 

  /**
   * @brief Sets this vector as a position.
   * @param position A vector representing absolute position.
   */
  void
    setPosition(const CVector2& position) {
    x = position.x;
    y = position.y;
  }

  /**
   * @brief Moves this vector by an offset.
   * @param offset A vector representing the amount to move.
   */
  void
    move(const CVector2& offset) {
    x += offset.x;
    y += offset.y;
  }

  /**
   * @brief Sets the scale of this vector.
   * @param factors A vector of scale factors.
   */
  void
    setScale(const CVector2& factors) {
    x = factors.x;
    y = factors.y;
  }

  /**
   * @brief Multiplies this vector by scale factors.
   * @param factors A vector of scale factors.
   */
  void
    scale(const CVector2& factors) {
    x *= factors.x;
    y *= factors.y;
  }

  /**
   * @brief Sets this vector as an origin point.
   * @param origin A vector representing origin.
   */
  void
    setOrigin(const CVector2& origin) {
    x = origin.x;
    y = origin.y;
  }


  //se va para utilidades


private:
  /**
   * @brief Returns the square root
   */
  float
    sqrt(float value) const {
    if (value <= 0.f) return 0.f;
    float
      guess = value / 2.f;
    for (int i = 0; i < 10; ++i) {
      guess = 0.5f * (guess + value / guess);
    }
    return guess;
  }
};