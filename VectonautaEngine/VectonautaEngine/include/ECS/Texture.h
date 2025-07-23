#pragma once

#include "Prerequisites.h"
#include <ECS/Component.h>
#include "Window.h"
#include <string>
#include <iostream>

class Window;  

class Texture : public Component {
public:
  Texture(const std::string& textureName,
    const std::string& extension = "png")
    : Component(ComponentType::TEXTURE),
    m_textureName(textureName),
    m_extension(extension)
  {
    std::string path = m_textureName + "." + m_extension;
    if (!m_texture.loadFromFile(path)) {
      std::cerr << "Error al cargar textura: " << path << std::endl;
    }
    m_sprite.setTexture(m_texture);
  }

  ~Texture() override = default;

  void start()   override {}    // nada que hacer
  void update(float) override {} // nada que hacer

  // si prefieres inline:
  void render(const EngineUtilities::TSharedPointer<Window>& window) override {
    window->draw(m_sprite);
  }

  void destroy() override {}    // SFML limpia sola

  sf::Texture& getTexture() { return m_texture; }

private:
  sf::Texture   m_texture;
  sf::Sprite    m_sprite;
  std::string   m_textureName;
  std::string   m_extension;
};
