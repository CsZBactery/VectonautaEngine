#pragma once
/**
 * @brief Cabecera principal de la aplicación del motor (loop, GUI y escena base).
 *
 * Administra la ventana, la GUI, los recursos, el circuito y los corredores.
 * Expone utilidades para inicializar/ejecutar/destruir y para aplicar una
 * ruta (path) editada a los corredores.
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
  * @brief Aplicación base que contiene el ciclo principal y estado de la escena.
  */
class BaseApp {
public:
  BaseApp() = default;
  ~BaseApp();

  /**
   * @brief Ejecuta el bucle principal de la app.
   * @return Código de salida (0 si todo fue bien).
   */
  int run();

  /**
   * @brief Inicializa ventana, GUI, recursos y crea la escena inicial.
   * @return true si la inicialización fue correcta.
   */
  bool init();

  /**
   * @brief Libera/termina los recursos de la aplicación.
   */
  void destroy();

private:
  /**
   * @brief Aplica un conjunto de puntos cerrados a los corredores.
   *
   * Densifica el path, genera carriles (offsets), posiciona a cada
   * corredor en su carril y hace reset de su estado.
   *
   * @param pts Puntos del circuito (deben formar un lazo; si no, se cierra).
   */
  void applyCurrentPathToRacers(const std::vector<sf::Vector2f>& pts);

private:
  // --- Infraestructura ---
  EngineUtilities::TSharedPointer<Window> m_windowPtr; ///< @brief Ventana principal.
  EngineGUI                                gui;        ///< @brief Paneles y controles de GUI.
  ResourceManager                          resourceMan;///< @brief Gestor de texturas/recursos.

  // --- Escena ---
  EngineUtilities::TSharedPointer<Actor>   m_trackActor; ///< @brief Actor del mapa/pista.
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;         ///< @brief Corredores activos.
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_finishedOrder;  ///< @brief Orden de llegada.

  // --- Carrera ---
  std::vector<sf::Vector2f> m_path;   ///< @brief Path base (carril 0, densificado).
  sf::FloatRect             m_finishLine; ///< @brief Recta de meta para conteo de vueltas.
};
