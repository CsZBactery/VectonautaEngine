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

  // Posición inicial en el primer punto
  if (!path.empty()) {
    if (auto xf = getComponent<Transform>()) {
      xf->setPosition(path.front());
      xf->setRotation(0.f);
    }
  }

  // << FIX >> que evita el 99% al inicio:
  // apuntemos al SEGUNDO waypoint como objetivo inicial
  currentWaypointIndex = (path.size() > 1 ? 1 : 0);
}

void A_Racer::reset() {
  // Misma idea del setPath: volver a inicio y apuntar al segundo punto
  m_currentLap = 0;
  m_place = 0;
  m_crossedLastFrame = false;

  if (!path.empty()) {
    if (auto xf = getComponent<Transform>()) {
      xf->setPosition(path.front());
      xf->setRotation(0.f);
    }
  }
  currentWaypointIndex = (path.size() > 1 ? 1 : 0);
}

float A_Racer::getProgress() const {
  // Progreso dentro de la vuelta (0..1)
  const int N = (int)path.size();
  if (N < 2) return 0.f;

  auto xf = const_cast<A_Racer*>(this)->getComponent<Transform>();
  sf::Vector2f pos = xf ? xf->getPosition() : path.front();

  const int cur = (currentWaypointIndex % N);
  const int prev = (cur + N - 1) % N;

  const sf::Vector2f A = path[prev];
  const sf::Vector2f B = path[cur];

  const float segLen = std::max(1e-4f, vlen(B - A));
  const float t = 1.f - clamp01(vlen(B - pos) / segLen); // 0 en A, 1 en B

  return clamp01((prev + t) / float(N));
}

void A_Racer::update(float deltaTime) {
  if (isFinished() || path.size() < 2) return;

  doPathFollowing(deltaTime);

  // Conteo de vueltas por cruce de línea
  bool inside = m_finishLine.contains(getComponent<Transform>()->getPosition());
  if (inside && !m_crossedLastFrame) {
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

  // Pure pursuit sencillo: mirar un poco más adelante del objetivo
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

    // Arrive: frena al acercarse al punto
    const float brakeRadius = lookaheadDistance * 1.5f;
    float speed = (d < brakeRadius) ? (m_maxSpeed * (d / brakeRadius)) : m_maxSpeed;

    pos += dir * speed * dt;

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    xf->setRotation(angleDeg);
    xf->setPosition(pos);
  }
}
