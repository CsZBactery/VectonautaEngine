#include "A_Racer.h"
#include "ECS/Transform.h"
#include <cmath>

A_Racer::A_Racer(const std::string& name, int playerId)
  : Actor(name)
{
  setPlayerId(playerId);
}

void A_Racer::setPath(const std::vector<sf::Vector2f>& pathPoints) {
  path = pathPoints;
  currentWaypointIndex = 0;
}

void A_Racer::reset() {
  currentWaypointIndex = 0;
  m_currentLap = 0;
  m_place = 0;
  m_crossedLastFrame = false;
  if (auto xf = getComponent<Transform>()) {
    if (!path.empty())
      xf->setPosition(path.front());
  }
}

float A_Racer::getProgress() const {
  if (path.size() < 2) return 0.f;
  float prog = static_cast<float>(currentWaypointIndex);
  // interpolación simple
  if (currentWaypointIndex + 1 < (int)path.size()) {
    if (auto xf = getComponent<Transform>()) {
      sf::Vector2f pos = xf->getPosition();
      sf::Vector2f a = path[currentWaypointIndex];
      sf::Vector2f b = path[currentWaypointIndex + 1];
      sf::Vector2f ab = b - a;
      float denom = ab.x * ab.x + ab.y * ab.y;
      if (denom > 0.0001f) {
        float t = ((pos.x - a.x) * ab.x + (pos.y - a.y) * ab.y) / denom;
        prog += std::clamp(t, 0.f, 1.f);
      }
    }
  }
  return prog / static_cast<float>(path.size() - 1);
}

void A_Racer::update(float deltaTime) {
  // 1) Path following
  doPathFollowing(deltaTime);

  // 2) Detección de meta / vueltas
  if (auto xf = getComponent<Transform>()) {
    bool nowCross = m_finishLine.contains(xf->getPosition());
    if (nowCross && !m_crossedLastFrame) {
      m_currentLap++;
    }
    m_crossedLastFrame = nowCross;
  }

  // 3) Aplica Transform->Shape/Texture
  Actor::update(deltaTime);
}

void A_Racer::doPathFollowing(float deltaTime) {
  if (path.size() < 2) return;
  if (auto xf = getComponent<Transform>()) {
    sf::Vector2f pos = xf->getPosition();
    // 1) Encontrar segmento más cercano
    int closest = 0; float minDist2 = 1e9f;
    for (int i = 0; i + 1 < (int)path.size(); ++i) {
      auto a = path[i], b = path[i + 1];
      sf::Vector2f ab = b - a;
      float len2 = ab.x * ab.x + ab.y * ab.y;
      if (len2 < 1e-6f) continue;
      float t = ((pos.x - a.x) * ab.x + (pos.y - a.y) * ab.y) / len2;
      t = std::clamp(t, 0.f, 1.f);
      sf::Vector2f proj = a + ab * t;
      sf::Vector2f diff = pos - proj;
      float d2 = diff.x * diff.x + diff.y * diff.y;
      if (d2 < minDist2) { minDist2 = d2; closest = i; }
    }
    currentWaypointIndex = closest;
    // 2) Objetivo adelantado
    float look = lookaheadDistance;
    sf::Vector2f target = path[closest];
    int idx = closest;
    while (look > 0.f && idx + 1 < (int)path.size()) {
      sf::Vector2f seg = path[idx + 1] - path[idx];
      float segLen = std::sqrt(seg.x * seg.x + seg.y * seg.y);
      if (segLen >= look) {
        target = path[idx] + seg * (look / segLen);
        break;
      }
      look -= segLen; ++idx;
    }
    // 3) Gira hacia target
    sf::Vector2f toT = target - pos;
    float ang = std::atan2(toT.y, toT.x) * 180.f / 3.14159265f;
    xf->setRotation(ang);
    // 4) Arrive
    xf->seek(target, m_maxSpeed, deltaTime, arriveRadius);
  }
}
