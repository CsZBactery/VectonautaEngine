#pragma once

/**
 * @file Actor.h
 * @brief Declara la clase Actor, entidad básica que contiene componentes (ECS) y puede renderizarse.
 */

#include <string>
#include <vector>
#include <type_traits>

#include "Prerequisites.h"
#include "Memory/TSharedPointer.h"
#include "ECS/Component.h"
#include "CShape.h"
#include "ECS/Transform.h"
#include "ECS/Texture.h"

class Window;

/**
 * @class Actor
 * @brief Actor básico que contiene componentes y puede representarse en el mundo.
 */
class Actor {
public:
  /**
   * @brief Construye un actor con nombre y añade por defecto Transform y CShape.
   * @param name Nombre del actor.
   */
  explicit Actor(const std::string& name)
    : m_name(name)
  {
    // Componentes base por defecto: shape y transform
    addComponent(EngineUtilities::MakeShared<CShape>());
    addComponent(EngineUtilities::MakeShared<Transform>());
  }

  /**
   * @brief Destructor virtual por defecto.
   */
  virtual ~Actor() = default;

  /**
   * @brief Hook de inicio; se llama una vez cuando el actor se inicializa.
   */
  virtual void start() {}

  /**
   * @brief Actualiza la lógica del actor cada frame.
   * @param deltaTime Tiempo transcurrido desde el último frame, en segundos.
   */
  virtual void update(float deltaTime);

  /**
   * @brief Dibuja el actor en la ventana indicada (formas, sprites, etc.).
   * @param window Ventana/render target donde se dibujará.
   */
  virtual void render(const EngineUtilities::TSharedPointer<Window>& window);

  /**
   * @brief Libera recursos asociados al actor (si aplica).
   */
  virtual void destroy() {}

  /**
   * @brief Obtiene el nombre del actor.
   * @return Referencia constante al nombre.
   */
  const std::string& getName() const { return m_name; }

  /**
   * @brief Cambia el nombre del actor.
   * @param n Nuevo nombre.
   */
  void setName(const std::string& n) { m_name = n; }

  /**
   * @brief Asigna el identificador de jugador asociado al actor.
   * @param id Identificador (0 si no aplica).
   */
  void setPlayerId(int id) { m_playerId = id; }

  /**
   * @brief Devuelve el identificador de jugador asociado.
   * @return ID de jugador.
   */
  int getPlayerId() const { return m_playerId; }

  /**
   * @brief Busca y devuelve el primer componente del tipo solicitado.
   * @tparam T Tipo de componente derivado de Component.
   * @return Shared pointer al componente encontrado o nulo si no existe.
   */
  template<typename T>
  EngineUtilities::TSharedPointer<T> getComponent() const {
    for (const auto& comp : components) {
      if (auto casted = comp.template dynamic_pointer_cast<T>()) {
        return casted;
      }
    }
    return EngineUtilities::TSharedPointer<T>();
  }

  /**
   * @brief Añade un componente al actor.
   * @tparam T Tipo del componente (debe derivar de Component).
   * @param component Shared pointer al componente a agregar.
   */
  template<typename T>
  void addComponent(const EngineUtilities::TSharedPointer<T>& component) {
    static_assert(std::is_base_of<Component, T>::value,
      "addComponent<T> sólo acepta Component derivados");
    EngineUtilities::TSharedPointer<Component> baseComp = component;
    components.push_back(baseComp);
  }

  /**
   * @brief Asigna una textura (componente Texture) al actor.
   * @param texture Componente de textura que envuelve un sf::Texture.
   */
  void setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

private:
  /** @brief Nombre del actor. */
  std::string m_name;

  /** @brief Lista de componentes que posee el actor. */
  std::vector<EngineUtilities::TSharedPointer<Component>> components;

  /** @brief Identificador de jugador (para UI/controles); 0 si no aplica. */
  int m_playerId = 0;
};
