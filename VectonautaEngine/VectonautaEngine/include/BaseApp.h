#pragma once
/**
 * @brief Cabecera principal de la aplicación (loop, GUI y escena base).
 *
 * - Administra ventana, GUI, recursos, pista y corredores.
 * - Permite elegir un corredor para controlarlo manualmente con teclado.
 */

#include "Prerequisites.h"
#include "Window.h"
#include "EngineGUI.h"
#include "ResourceManager.h"
#include "ECS/Texture.h"
#include "A_Racer.h"

#include <SFML/Graphics.hpp>
#include <vector>

 /**
  * @brief Aplicación base con ciclo principal y estado de escena/carrera.
  */
class BaseApp {
public:
  BaseApp() = default;
  ~BaseApp();

  /**
   * @brief Ejecuta el bucle principal.
   * @return 0 si todo va bien.
   */
  int run();

  /**
   * @brief Inicializa ventana, GUI, recursos y escena.
   */
  bool init();

  /**
   * @brief Libera recursos.
   */
  void destroy();

private:
  /**
   * @brief Aplica un path a los corredores (densifica + carriles + posiciona).
   */
  void applyCurrentPathToRacers(const std::vector<sf::Vector2f>& pts);

  /**
   * @brief Control del jugador (arcade) cuando hay corredor seleccionado.
   */
  void updatePlayerControl(float dt);

private:
  // --- Infraestructura ---
  EngineUtilities::TSharedPointer<Window> m_windowPtr;
  EngineGUI   gui;
  ResourceManager resourceMan;

  // --- Escena ---
  EngineUtilities::TSharedPointer<Actor>   m_trackActor;
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_finishedOrder;

  // --- Carrera ---
  std::vector<sf::Vector2f> m_path;
  sf::FloatRect             m_finishLine;

  // --- Jugador ---
  int           m_playerIdx = -1;     // -1 = nadie
  sf::Vector2f  m_playerVel = { 0.f,0.f };
  float         m_playerAng = 0.f;     // radianes
  float         m_playerAccel = 480.f; // px/s^2
  float         m_playerTurn = 2.6f;  // rad/s (a velocidad alta gira más)
  float         m_playerMaxSp = 380.f; // px/s
  float         m_playerDrag = 0.90f; // fricción por frame
};
