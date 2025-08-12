#pragma once

#include "ECS/Actor.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <string>

/**
 * @class A_Racer
 * @brief Racer (NPC/Jugador). Lleva path following, vueltas y posición final.
 */
class A_Racer : public Actor {
public:
  /**
   * @brief Crea un corredor con nombre y un id opcional de jugador.
   * @param name Nombre visible del corredor.
   * @param playerId Índice de jugador (0 si es NPC).
   */
  explicit A_Racer(const std::string& name, int playerId = 0);

  /**
   * @brief Hook de inicio (vacío si no se usa).
   */
  void start() override {}

  /**
   * @brief Actualiza el estado del corredor cada frame.
   * @param deltaTime Tiempo transcurrido en segundos.
   */
  void update(float deltaTime) override;

  /**
   * @brief Asigna el path (lista de puntos) que el corredor debe seguir.
   * @param pathPoints Puntos (en orden) del trazado.
   */
  void setPath(const std::vector<sf::Vector2f>& pathPoints);

  /**
   * @brief Reinicia el estado del corredor al inicio del path.
   */
  void reset();

  /**
   * @brief Define el rectángulo de meta para contar vueltas.
   * @param rect Área de meta (sf::FloatRect).
   */
  void setFinishLine(const sf::FloatRect& rect) { m_finishLine = rect; }

  /**
   * @brief Fija el número total de vueltas de la carrera.
   * @param laps Vueltas totales.
   */
  void setTotalLaps(int laps) { m_totalLaps = laps; }

  /**
   * @brief Devuelve la vuelta actual (0..total).
   * @return Vuelta alcanzada.
   */
  int  getCurrentLap() const { return m_currentLap; }

  /**
   * @brief Devuelve el total de vueltas configurado.
   * @return Vueltas totales.
   */
  int  getTotalLaps()  const { return m_totalLaps; }

  /**
   * @brief Indica si el corredor ya terminó todas sus vueltas.
   * @return true si terminó, false en caso contrario.
   */
  bool isFinished()    const { return m_currentLap >= m_totalLaps; }

  /**
   * @brief Configura la velocidad máxima (px/s) del corredor.
   * @param s Velocidad máxima.
   */
  void  setMaxSpeed(float s) { m_maxSpeed = s; }

  /**
   * @brief Obtiene la velocidad máxima configurada.
   * @return Velocidad máxima (px/s).
   */
  float getMaxSpeed() const { return m_maxSpeed; }

  /**
   * @brief Progreso normalizado de la vuelta actual.
   * @return Valor en [0..1].
   */
  float getProgress() const;   // 0..1 del loop actual (decl; impl en .cpp)

  /**
   * @brief Posición final de podio (1..N), 0 si sigue corriendo.
   * @return Place actual.
   */
  int   getPlace() const { return m_place; }

  /**
   * @brief Asigna la posición de podio (1..N) cuando finaliza.
   * @param p Posición final.
   */
  void  setPlace(int p) { m_place = p; }

private:
  /**
   * @brief Lógica de seguimiento del path (steering hacia waypoints).
   * @param deltaTime Tiempo transcurrido en segundos.
   */
  void doPathFollowing(float deltaTime);

  // --- Ruta ---
  /** @brief Puntos del trazado a seguir. */
  std::vector<sf::Vector2f> path;
  /** @brief Índice del waypoint actual. */
  int   currentWaypointIndex = 0;

  // --- Parámetros de steering ---
  /** @brief Distancia de mirada por delante (pure pursuit). */
  float lookaheadDistance = 90.f;
  /** @brief Radio para considerar alcanzado el waypoint. */
  float arriveRadius = 18.f;
  /** @brief Velocidad máxima en píxeles/segundo. */
  float m_maxSpeed = 160.f;

  // --- Meta / vueltas ---
  /** @brief Rectángulo de meta usado para contar vueltas. */
  sf::FloatRect m_finishLine{};
  /** @brief Vuelta actual. */
  int  m_currentLap = 0;
  /** @brief Vueltas totales a completar. */
  int  m_totalLaps = 3;
  /** @brief Flag para detectar flanco de cruce de meta. */
  bool m_crossedLastFrame = false;

  // --- Estado de carrera ---
  /** @brief Posición de podio (0 si aún no termina). */
  int  m_place = 0;
  /** @brief Índice de jugador (para UI o input), opcional. */
  int  m_playerIndex = 0;
};
