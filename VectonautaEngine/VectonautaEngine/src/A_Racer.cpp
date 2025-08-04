#include "A_Racer.h"
#include "ECS/Transform.h"
#include "CShape.h"
#include "Window.h"
#include <cmath>
#include <algorithm>

A_Racer::A_Racer(const std::string& name, int playerId)
  : Actor(name)
{
  setPlayerId(playerId);
}

void A_Racer::start() {
  // Inicializaciones adicionales si se requieren
}

void A_Racer::update(float deltaTime) {
  // Aplica steering (seguimiento de ruta)
  doPathFollowing(deltaTime);

  // Luego aplica la lógica base (sync transform -> shape/texture)
  Actor::update(deltaTime);
}

void A_Racer::render(const EngineUtilities::TSharedPointer<Window>& window) {
  Actor::render(window);
}

void A_Racer::setPath(const std::vector<sf::Vector2f>& pathPoints) {
  path = pathPoints;
  currentWaypointIndex = 0;

  // Colocar en inicio inmediato
  if (auto xf = getComponent<Transform>()) {
    if (!path.empty()) {
      xf->setPosition(path[0]);
    }
  }
}

void A_Racer::reset() {
  currentWaypointIndex = 0;
  place = 0;
  if (auto xf = getComponent<Transform>()) {
    if (!path.empty())
      xf->setPosition(path[0]);
  }
}

float A_Racer::getProgress() const {
  if (path.empty()) return 0.f;
  float progress = static_cast<float>(currentWaypointIndex);
  if (currentWaypointIndex + 1 < static_cast<int>(path.size())) {
    if (auto xf = getComponent<Transform>()) {
      sf::Vector2f pos = xf->getPosition();
      sf::Vector2f a = path[currentWaypointIndex];
      sf::Vector2f b = path[currentWaypointIndex + 1];
      sf::Vector2f ab = b - a;
      float abLenSq = ab.x * ab.x + ab.y * ab.y;
      if (abLenSq > 0.0001f) {
        float t = ((pos.x - a.x) * ab.x + (pos.y - a.y) * ab.y) / abLenSq;
        t = std::clamp(t, 0.f, 1.f);
        progress += t;
      }
    }
  }
  int segments = std::max(1, static_cast<int>(path.size() - 1));
  return progress / static_cast<float>(segments); // normalizado 0..1
}

void A_Racer::doPathFollowing(float deltaTime) {
  if (path.size() < 2) return;
  if (auto xf = getComponent<Transform>()) {
    sf::Vector2f pos = xf->getPosition();

    // 1. Encontrar el segmento más cercano
    int closestIndex = 0;
    float minDistSq = std::numeric_limits<float>::max();
    for (int i = 0; i + 1 < static_cast<int>(path.size()); ++i) {
      sf::Vector2f a = path[i];
      sf::Vector2f b = path[i + 1];
      sf::Vector2f ab = b - a;
      float abLenSq = ab.x * ab.x + ab.y * ab.y;
      if (abLenSq == 0) continue;
      float t = ((pos.x - a.x) * ab.x + (pos.y - a.y) * ab.y) / abLenSq;
      t = std::clamp(t, 0.f, 1.f);
      sf::Vector2f projection = a + ab * t;
      sf::Vector2f diff = pos - projection;
      float distSq = diff.x * diff.x + diff.y * diff.y;
      if (distSq < minDistSq) {
        minDistSq = distSq;
        closestIndex = i;
      }
    }

    currentWaypointIndex = closestIndex;

    // 2. Calcular objetivo adelantado (lookahead) en la ruta
    float remainingLook = lookaheadDistance;
    sf::Vector2f target = path[closestIndex];
    int idx = closestIndex;
    while (remainingLook > 0.f && idx + 1 < static_cast<int>(path.size())) {
      sf::Vector2f segment = path[idx + 1] - path[idx];
      float segLen = std::sqrt(segment.x * segment.x + segment.y * segment.y);
      if (segLen >= remainingLook) {
        sf::Vector2f dir = segment / segLen;
        target = path[idx] + dir * remainingLook;
        break;
      }
      else {
        remainingLook -= segLen;
        ++idx;
      }
    }

    // 3. Orientarse hacia el target
    sf::Vector2f toTarget = target - pos;
    float angleDegrees = std::atan2(toTarget.y, toTarget.x) * 180.f / 3.14159265f;
    xf->setRotation(angleDegrees);

    // 4. Mover con arrive/seek suavizado
    xf->seek(target, maxSpeed, deltaTime, arriveRadius);
  }
}
