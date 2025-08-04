#pragma once

#include "ECS/Actor.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

/**
 * @brief Racer NPC / jugador en la pista. Gestiona place y steering.
 */
class A_Racer : public Actor {
public:
  explicit A_Racer(const std::string& name, int playerId = 0);

  void start() override;
  void update(float deltaTime) override;
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  // Path que sigue el racer (debe asignarse desde fuera)
  void setPath(const std::vector<sf::Vector2f>& pathPoints);

  // Reinicia al inicio
  void reset();

  // Getters / setters
  int getPlace() const { return place; }
  void setPlace(int p) { place = p; }

  int getCurrentWaypointIndex() const { return currentWaypointIndex; }
  void setSpeed(float s) { maxSpeed = s; }
  float getSpeed() const { return maxSpeed; }

  // Obtener progreso relativo (0..1) en la pista para ranking visual
  float getProgress() const;

private:
  // Steering interno (path following)
  void doPathFollowing(float deltaTime);

  std::vector<sf::Vector2f> path; // ruta completa
  int currentWaypointIndex = 0;   // segmento actual
  float lookaheadDistance = 50.f; // cuánto "mira" hacia adelante
  float arriveRadius = 10.f;      // radio de llegada suavizada
  float maxSpeed = 200.f;         // velocidad base

  int place = 0; // 1 = primero, 2 = segundo, etc.
};
