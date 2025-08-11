#include "ECS/Texture.h"
#include "Window.h"
#include <iostream>

static std::string makeFullPath(const std::string& base, const std::string& ext) {
  // WD esperado: $(ProjectDir)
  return std::string("bin/") + base + "." + ext;
}

Texture::Texture(const std::string& textureName, const std::string& extension)
  : m_name(textureName), m_ext(extension) {
  const std::string path = makeFullPath(m_name, m_ext);
  if (!m_texture.loadFromFile(path)) {
    m_sprite.reset();
    std::cerr << "[Texture] Cannot load: " << path << "\n";
    return;
  }

  // Crea el sprite y centra origen (SFML 3)
  m_sprite.emplace(m_texture); // ¡l-value, no temporales!

  const auto bounds = m_sprite->getLocalBounds();
  const auto sz = bounds.size;          // <- SFML 3: miembro .size (no .size())
  m_sprite->setOrigin(sf::Vector2f{ sz.x * 0.5f, sz.y * 0.5f });
}

void Texture::setPosition(const sf::Vector2f& p) {
  if (m_sprite) m_sprite->setPosition(p);
}

void Texture::setRotation(float degrees) {
  if (m_sprite) m_sprite->setRotation(sf::degrees(degrees)); // <- sf::Angle
}

void Texture::setScale(const sf::Vector2f& s) {
  if (m_sprite) m_sprite->setScale(s);
}

void Texture::render(const EngineUtilities::TSharedPointer<Window>& window) {
  if (!m_sprite) return;
  window->draw(*m_sprite);
}
