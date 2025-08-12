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

  if (!path.empty()) {
    if (auto xf = getComponent<Transform>()) {
      xf->setPosition(path.front());
      xf->setRotation(0.f);
    }
  }
  currentWaypointIndex = (path.size() > 1 ? 1 : 0);

  // <-- sincroniza sprite con el Transform inicial
  Actor::update(0.f);
}

void A_Racer::reset() {
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

  // <-- sincroniza sprite tras el reset
  Actor::update(0.f);
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
  if (!isFinished() && path.size() >= 2) {
    doPathFollowing(deltaTime);

    bool inside = m_finishLine.contains(getComponent<Transform>()->getPosition());
    if (inside && !m_crossedLastFrame) ++m_currentLap;
    m_crossedLastFrame = inside;
  }

  // <-- clave: copiar Transform -> sprite cada frame
  Actor::update(deltaTime);
}

void A_Racer::doPathFollowing(float dt) {
  if (path.size() < 2) return;
  auto xf = getComponent<Transform>();
  if (!xf) return;

  sf::Vector2f pos = xf->getPosition();

  // 1) Si estoy MUY cerca del waypoint actual, avanza varios para no quedarte pegado
  int guard = 0;
  while (guard < 8) {
    sf::Vector2f toCurr = path[currentWaypointIndex] - pos;
    if (std::sqrt(toCurr.x * toCurr.x + toCurr.y * toCurr.y) > arriveRadius) break;
    currentWaypointIndex = (currentWaypointIndex + 1) % (int)path.size();
    ++guard;
  }

  // 2) Pure-pursuit simplificado: mira K puntos por delante (según densidad ~30px)
  constexpr float approxSegLen = 30.f; // igual a la densificación que usamos
  int K = std::max(1, (int)std::round(lookaheadDistance / approxSegLen));
  int aheadIdx = (currentWaypointIndex + K) % (int)path.size();
  sf::Vector2f target = path[aheadIdx];

  sf::Vector2f to = target - pos;
  float d = std::sqrt(to.x * to.x + to.y * to.y);
  if (d < 1e-4f) return;

  sf::Vector2f dir = { to.x / d, to.y / d };

  // 3) Frenado suave al acercarse al target de lookahead
  float brakeRadius = lookaheadDistance * 1.2f;
  float speed = (d < brakeRadius) ? (m_maxSpeed * (d / brakeRadius)) : m_maxSpeed;

  pos += dir * speed * dt;

  float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
  xf->setRotation(angleDeg);
  xf->setPosition(pos);
}
