#pragma once
/**
 * @file Entity.h
 * @brief Declara la interfaz base Entity para objetos con ciclo de vida ECS (start/update/render/destroy) y manejo de componentes.
 */

#include <Prerequisites.h>
#include <ECS/Component.h>

class
  Window;

/**
 * @class Entity
 * @brief Interfaz abstracta de una entidad que puede inicializarse, actualizarse, renderizarse y destruirse, además de contener componentes ECS.
 */
class
  Entity {
public:

  /**
   * @brief Destructor virtual por defecto.
   */
  virtual
    ~Entity() = default;

  /**
   * @brief Método puro virtual para la lógica de inicialización (se llama una vez).
   */
  virtual void
    start() = 0;

  /**
   * @brief Método puro virtual para actualizar la lógica cada frame.
   * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
   */
  virtual void
    update(float deltaTime) = 0;

  /**
   * @brief Método puro virtual para dibujar la entidad/componente.
   * @param window Smart pointer a la ventana donde se renderiza.
   */
  virtual void
    render(const EngineUtilities::TSharedPointer<Window>& window) = 0;

  /**
   * @brief Método puro virtual para liberar recursos y realizar limpieza.
   */
  virtual void
    destroy() = 0;

  /**
   * @brief Agrega un componente derivado de Component a la entidad.
   * @tparam T Tipo del componente (debe derivar de Component).
   * @param component Shared pointer al componente a agregar.
   */
  template<typename T> void
    addComponent(EngineUtilities::TSharedPointer<T> component) {
    static_assert(std::is_base_of<Component, T>
      ::value, "T must be derived from Component");
    components.push_back
    (component.template dynamic_pointer_cast<Component>());
  }

  /**
   * @brief Obtiene el primer componente del tipo solicitado si existe.
   * @tparam T Tipo del componente a buscar.
   * @return Shared pointer al componente encontrado o nulo si no se halla.
   */
  template<typename T>
  EngineUtilities::TSharedPointer<T>
    getComponent() {
    for (auto& component : components) {
      EngineUtilities::TSharedPointer<T> specificComponent
        = component.template dynamic_pointer_cast<T>();
      if (specificComponent) {
        return specificComponent;
      }
    }

    return EngineUtilities::TSharedPointer<T>();
  }

protected:
  /** @brief Bandera de actividad de la entidad (true si está activa). */
  bool isActive;
  /** @brief Identificador único de la entidad. */
  uint32_t id;
  /** @brief Lista de componentes asociados a la entidad. */
  std::vector < EngineUtilities::TSharedPointer<Component>> components;
};
