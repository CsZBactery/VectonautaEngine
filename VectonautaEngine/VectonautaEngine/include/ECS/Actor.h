#pragma once
#include "../Prerequisites.h"
#include "Entity.h"
#include "CShape.h"
#include "Transform.h"

class
  Actor : public Entity {
public:
  Actor() = default;
  Actor(const std::string& actorName);

  virtual
    ~Actor() = default;

  void
    start() override;

  void
    update(float deltaTime) override;

  void
    render(const EngineUtilities::TSharedPointer<Window>& window) override;

  void
    destroy() override;

private:
  std::string m_name = "Actor";

  template <typename T>
  inline EngineUtilities::TSharedPointer<T> getComponents() {
    for (auto& component : components) {
      EngineUtilities::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
      if (specificComponent) {
        return specificComponent;
      }
    }
    return EngineUtilities::TSharedPointer<T>();
  }
};
