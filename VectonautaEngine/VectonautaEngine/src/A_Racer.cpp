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
  auto xf = getComponent<Transform>();
  if (!xf || path.size() < 2) return;

  sf::Vector2f pos = xf->getPosition();

  // Segmento actual A->B
  const int N = (int)path.size();
  int i = currentWaypointIndex;
  sf::Vector2f A = path[i];
  sf::Vector2f B = path[(i + 1) % N];
  sf::Vector2f AB = B - A;
  float abLen2 = AB.x * AB.x + AB.y * AB.y;
  if (abLen2 < 1e-6f) { currentWaypointIndex = (i + 1) % N; return; }
  float abLen = std::sqrt(abLen2);

  // Proyección del coche sobre el segmento A->B
  sf::Vector2f AP = pos - A;
  float t = (AP.x * AB.x + AP.y * AB.y) / abLen2; // 0..1 si estás dentro del segmento

  // Regla de avance de waypoint:
  // - Si ya pasaste B (t > 1), o
  // - Si estás suficientemente cerca de B,
  //   avanza al siguiente
  float distToB = std::hypot(pos.x - B.x, pos.y - B.y);
  if (t > 1.f || distToB < arriveRadius) {
    currentWaypointIndex = (i + 1) % N;
    i = currentWaypointIndex;
    A = path[i];
    B = path[(i + 1) % N];
    AB = B - A;
    abLen2 = AB.x * AB.x + AB.y * AB.y;
    abLen = std::sqrt(abLen2);
    if (abLen2 < 1e-6f) return;
    AP = pos - A;
    t = (AP.x * AB.x + AP.y * AB.y) / abLen2;
  }

  // Punto de persecución (pure pursuit) a partir de la proyección + lookahead
  float s = std::clamp(t + (lookaheadDistance / std::max(abLen, 1e-3f)), 0.f, 1.f);
  sf::Vector2f pursue = A + AB * s;

  // Velocidad con frenado suave al aproximarse al punto de persecución
  sf::Vector2f to = pursue - pos;
  float d = std::hypot(to.x, to.y);
  if (d > 1e-4f) {
    sf::Vector2f dir = { to.x / d, to.y / d };

    float brakeRadius = lookaheadDistance * 1.2f;
    float speed = (d < brakeRadius) ? (m_maxSpeed * (d / brakeRadius)) : m_maxSpeed;

    pos += dir * speed * dt;

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    xf->setRotation(angleDeg + m_spriteAngleOffset);
    xf->setPosition(pos);
  }
}
