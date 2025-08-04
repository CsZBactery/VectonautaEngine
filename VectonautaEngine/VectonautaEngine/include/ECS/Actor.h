#pragma once

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
 * @brief Actor básico que contiene componentes y puede representarse en el mundo.
 */
class Actor {
public:
  explicit Actor(const std::string& name)
    : m_name(name)
  {
    // Componentes base por defecto: shape y transform
    addComponent(EngineUtilities::MakeShared<CShape>());
    addComponent(EngineUtilities::MakeShared<Transform>());
  }

  virtual ~Actor() = default;

  virtual void start() {}
  virtual void update(float deltaTime);
  virtual void render(const EngineUtilities::TSharedPointer<Window>& window);
  virtual void destroy() {}

  const std::string& getName() const { return m_name; }
  void setName(const std::string& n) { m_name = n; }

  void setPlayerId(int id) { m_playerId = id; }
  int getPlayerId() const { return m_playerId; }

  template<typename T>
  EngineUtilities::TSharedPointer<T> getComponent() const {
    for (const auto& comp : components) {
      if (auto casted = comp.template dynamic_pointer_cast<T>()) {
        return casted;
      }
    }
    return EngineUtilities::TSharedPointer<T>();
  }

  template<typename T>
  void addComponent(const EngineUtilities::TSharedPointer<T>& component) {
    static_assert(std::is_base_of<Component, T>::value,
      "addComponent<T> sólo acepta Component derivados");
    EngineUtilities::TSharedPointer<Component> baseComp = component;
    components.push_back(baseComp);
  }

  void setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

private:
  std::string m_name;
  std::vector<EngineUtilities::TSharedPointer<Component>> components;
  int m_playerId = 0;
};
