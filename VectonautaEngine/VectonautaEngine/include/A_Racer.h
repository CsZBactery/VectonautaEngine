#pragma once

#include "ECS/Actor.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <string>

/**
 * @brief Racer (NPC/Jugador). Lleva path following, vueltas y posición final.
 */
class A_Racer : public Actor {
public:
  explicit A_Racer(const std::string& name, int playerId = 0);

  void start() override {}                 // si no usas start, lo dejamos vacío
  void update(float deltaTime) override;   // implementado en A_Racer.cpp

  // Path a seguir (asígnalo desde BaseApp)
  void setPath(const std::vector<sf::Vector2f>& pathPoints);

  // Reinicia estado (vuelve al inicio del path)
  void reset();

  // Línea de meta y vueltas
  void setFinishLine(const sf::FloatRect& rect) { m_finishLine = rect; }
  void setTotalLaps(int laps) { m_totalLaps = laps; }
  int  getCurrentLap() const { return m_currentLap; }
  int  getTotalLaps()  const { return m_totalLaps; }
  bool isFinished()    const { return m_currentLap >= m_totalLaps; }

  // Velocidad máxima (para diferenciar corredores)
  void  setMaxSpeed(float s) { m_maxSpeed = s; }
  float getMaxSpeed() const { return m_maxSpeed; }

  // Podio / progreso
  int   getPlace() const { return m_place; }
  void  setPlace(int p) { m_place = p; }
  float getProgress() const;   // 0..1 del loop actual (decl; impl en .cpp)

private:
  void doPathFollowing(float deltaTime);   // steering hacia waypoints (impl en .cpp)

  // --- Ruta ---
  std::vector<sf::Vector2f> path;
  int   currentWaypointIndex = 0;

  // --- Parámetros de steering ---
  float lookaheadDistance = 90.f;   // pure pursuit
  float arriveRadius = 18.f;   // cambiar de waypoint
  float m_maxSpeed = 160.f;  // px/s

  // --- Meta / vueltas ---
  sf::FloatRect m_finishLine{};
  int  m_currentLap = 0;
  int  m_totalLaps = 3;
  bool m_crossedLastFrame = false;

  // --- Estado de carrera ---
  int  m_place = 0;        // 0 = corriendo; 1..N = posición final
  int  m_playerIndex = 0;  // opcional, por si lo usas en GUI
};
