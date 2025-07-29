#pragma once

/**
 * @file Texture.h
 * @brief Component that loads and renders an SFML texture via a sprite.
 */

#include "Prerequisites.h"       // Core utilities and SFML headers
#include "ECS/Component.h"       // Base Component class
#include "Window.h"              // Window for rendering
#include <string>                 // std::string
#include <iostream>               // std::cerr

class Window;  // Forward declaration

/**
 * @class Texture
 * @brief Component for loading a texture and drawing it as an SFML sprite.
 */
class Texture : public Component {
public:
  /**
   * @brief Loads a texture from file and prepares its sprite.
   * @param textureName Base filename (without extension).
   * @param extension File extension (default: "png").
   */
  Texture(const std::string& textureName,
    const std::string& extension = "png")
    : Component(ComponentType::TEXTURE)
    , m_textureName(textureName)
    , m_extension(extension)
  {
    std::string path = m_textureName + "." + m_extension;
    if (!m_texture.loadFromFile(path)) {
      std::cerr << "Error loading texture: " << path << std::endl;
    }
    m_sprite.setTexture(m_texture);
  }

  ~Texture() override = default;

  void start()   override {}
  void update(float /*dt*/) override {}

  /**
   * @brief Draws the sprite to the given window.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override {
    if (window) {
      window->draw(m_sprite);
    }
  }

  void destroy() override {}

  /**
   * @brief Returns the underlying SFML texture.
   */
  sf::Texture& getTexture() { return m_texture; }

private:
  sf::Texture   m_texture;      ///< SFML texture resource
  sf::Sprite    m_sprite;       ///< Sprite for rendering the texture
  std::string   m_textureName;  ///< Filename (without extension)
  std::string   m_extension;    ///< File extension used
};
