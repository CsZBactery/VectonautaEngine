#pragma once

#include "ECS/Actor.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

/**
 * @brief Racer NPC / jugador en la pista. Gestiona place, laps y steering.
 */
class A_Racer : public Actor {
public:
  explicit A_Racer(const std::string& name, int playerId = 0);

  void start() override {}
  void update(float deltaTime) override;
  void render(const EngineUtilities::TSharedPointer<Window>& window) override {
    Actor::render(window);
  }

  // Path que sigue el racer (debe asignarse desde fuera)
  void setPath(const std::vector<sf::Vector2f>& pathPoints);

  // Reinicia al inicio
  void reset();

  // Línea de meta
  void setFinishLine(const sf::FloatRect& rect) { m_finishLine = rect; }

  // Configuración de race
  void setTotalLaps(int laps) { m_totalLaps = laps; }
  int  getCurrentLap() const { return m_currentLap; }
  int  getTotalLaps() const { return m_totalLaps; }
  bool isFinished()  const { return m_currentLap >= m_totalLaps; }

  // Getters / setters básicos
  int  getPlace() const { return m_place; }
  void setPlace(int p) { m_place = p; }
  float getProgress() const;   // Normalizado 0..1

private:
  void doPathFollowing(float deltaTime);

  std::vector<sf::Vector2f> path;      ///< ruta completa
  int currentWaypointIndex = 0;        ///< segmento actual

  // Parámetros de steering
  float lookaheadDistance = 50.f;
  float arriveRadius = 10.f;
  float m_maxSpeed = 200.f;

  // Meta/laps
  sf::FloatRect m_finishLine;
  int    m_currentLap = 0;
  int    m_totalLaps = 3;
  bool   m_crossedLastFrame = false;

  // Estado de carrera
  int m_place = 0;  ///< posición final (1,2,3...)
};
