#pragma once

/**
 * @file Component.h
 * @brief Declares the base Component class and ComponentType enumeration for entity components.
 */

#include <Prerequisites.h>

class
  Window;

/**
 * @enum ComponentType
 * @brief Enumeration of all possible component types used in the entity-component system.
 */
enum
  ComponentType {
  None = 0,       ///< No component
  TRANSFORM = 1,   ///< Transform component (position, rotation, scale)
  SRPITE = 2,     ///< Sprite component (2D image)
  RENDERER = 3,   ///< Renderer component
  PHYSICS = 4,    ///< Physics simulation component
  AUDIOSOURCE = 5,///< Audio source component
  SHAPE = 6,      ///< Shape component (geometry-based)
  TEXTURE = 7     ///< Texture component (for applying textures)
};

/**
 * @class Component
 * @brief Abstract base class for all components in the game engine.
 *
 * Components represent behavior or data associated with game objects. This class provides
 * virtual methods that must be overridden by derived components to define behavior.
 */
class
  Component {
public:
  /**
   * @brief Default constructor.
   */
  Component() = default;

  /**
   * @brief Constructor that sets the component type.
   * @param type Type of the component as defined in ComponentType enum.
   */
  Component(const ComponentType type) : m_type(type) {}

  /**
   * @brief Virtual destructor.
   */
  virtual
    ~Component() = default;

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

  /**
   * @brief Gets the type of the component.
   * @return The component type (enum value).
   */
  ComponentType
    getType() const { return m_type; }

protected:
  ComponentType m_type; ///< The specific type of the component.
};