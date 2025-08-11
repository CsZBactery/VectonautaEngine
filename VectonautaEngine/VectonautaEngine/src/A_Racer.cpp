// A_Racer.cpp
#include "A_Racer.h"
#include "ECS/Transform.h"
#include <cmath>
#include <algorithm>

static inline float vlen(const sf::Vector2f& v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}
static inline sf::Vector2f vnorm(const sf::Vector2f& v) {
  float L = vlen(v);
  return (L > 1e-5f) ? sf::Vector2f{ v.x / L, v.y / L } : sf::Vector2f{ 0.f, 0.f };
}
static inline float clamp01(float x) { return std::max(0.f, std::min(1.f, x)); }

A_Racer::A_Racer(const std::string& name, int /*playerId*/)
  : Actor(name) {
}

void A_Racer::setPath(const std::vector<sf::Vector2f>& pathPoints) {
  path = pathPoints;
  currentWaypointIndex = 0;

  // Colócalo al inicio del camino
  if (!path.empty()) {
    if (auto xf = getComponent<Transform>()) {
      xf->setPosition(path.front());
      xf->setRotation(0.f);
    }
  }
}

void A_Racer::reset() {
  currentWaypointIndex = 0;
  m_currentLap = 0;
  m_place = 0;
  m_crossedLastFrame = false;

  if (!path.empty()) {
    if (auto xf = getComponent<Transform>()) {
      xf->setPosition(path.front());
      xf->setRotation(0.f);
    }
  }
}

float A_Racer::getProgress() const {
  if (path.size() < 2) return float(m_currentLap) / std::max(1, m_totalLaps);

  // Aproximación: progreso = (waypoints completados + fracción del segmento actual) / |path|
  auto xf = const_cast<A_Racer*>(this)->getComponent<Transform>();
  sf::Vector2f pos = xf ? xf->getPosition() : path[currentWaypointIndex];

  int prev = (currentWaypointIndex - 1 + int(path.size())) % int(path.size());
  const sf::Vector2f A = path[prev];
  const sf::Vector2f B = path[currentWaypointIndex];

  float segLen = std::max(1e-4f, vlen(B - A));
  float frac = 1.f - clamp01(vlen(B - pos) / segLen);

  float loopFrac = (float(prev) + frac) / float(path.size());
  // Si llevas vueltas, suma (solo informativo)
  return clamp01(loopFrac);
}

void A_Racer::update(float deltaTime) {
  if (isFinished() || path.size() < 2) return;

  doPathFollowing(deltaTime);

  // --- Conteo de vueltas por cruce de meta ---
  bool inside = m_finishLine.contains(getComponent<Transform>()->getPosition());
  if (inside && !m_crossedLastFrame) {
    // Entró a la zona de meta este frame -> nueva vuelta
    ++m_currentLap;
  }
  m_crossedLastFrame = inside;
}

void A_Racer::doPathFollowing(float dt) {
  auto xf = getComponent<Transform>();
  if (!xf) return;

  sf::Vector2f pos = xf->getPosition();
  sf::Vector2f target = path[currentWaypointIndex];
  sf::Vector2f to = target - pos;
  float d = vlen(to);

  // ¿Llegamos al waypoint?
  if (d < arriveRadius) {
    currentWaypointIndex = (currentWaypointIndex + 1) % int(path.size());
    target = path[currentWaypointIndex];
    to = target - pos;
    d = vlen(to);
  }

  // Punto de mirada adelante para suavizar curvas (pure-pursuit sencillo)
  if (path.size() >= 2) {
    int next = (currentWaypointIndex + 1) % int(path.size());
    sf::Vector2f dirSeg = vnorm(path[next] - path[currentWaypointIndex]);
    float extra = std::max(0.f, lookaheadDistance - std::min(d, lookaheadDistance));
    sf::Vector2f lookPt = target + dirSeg * extra;
    to = lookPt - pos;
    d = vlen(to);
  }

  if (d > 1e-4f) {
    sf::Vector2f dir = vnorm(to);

    // Arrive: frena al acercarse al punto de mira
    float brakeRadius = lookaheadDistance * 1.5f;
    float speed = (d < brakeRadius) ? (m_maxSpeed * (d / brakeRadius)) : m_maxSpeed;

    pos += dir * speed * dt;

    // Orientación del sprite
    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    xf->setRotation(angleDeg);
    xf->setPosition(pos);
  }
}
