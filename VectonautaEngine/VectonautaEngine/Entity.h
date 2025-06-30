#pragma once
#include "..//Prerequisites.h"
#include "Component.h"

class
  Window;

class
  Entity {
public:


  virtual
    ~Entity() = default;

  /**
   * @brief Pure virtual method for initialization logic.
   * @param deltaTime Time elapsed since last frame (used for time-dependent setup).
   */
  virtual void
    start() = 0;

  /**
   * @brief Pure virtual method for updating logic every frame.
   * @param deltaTime Time elapsed since last frame.
   */
  virtual void
    update(float deltaTime) = 0;

  /**
   * @brief Pure virtual method for rendering the component.
   * @param window Smart pointer to the window where rendering occurs.
   */
  virtual void
    render(const EngineUtilities::TSharedPointer<Window>& window) = 0;

  /**
   * @brief Pure virtual method for cleaning up resources.
   */
  virtual void
    destroy() = 0;


  template<typename T>
  void addComponent(EngineUtilities::TSharedPointer<T> component) {
    static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
    component.push_back(component.template dynamic_pointer_cast<Component>());
  }

  template<typename T>
  EngineUtilities::TSharedPointer<T>
    getComponents() {
    for (auto& component : components) {
      EngineUtilities::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
      if (specificComponent) {
        return specificComponent;
      }
    }
    return EngineUtilities::TSharedPointer<T>();
  }

protected:
  bool isActive;
  uint32_t id;
  std::vector < EngineUtilities::TSharedPointer<Component>> components;
};