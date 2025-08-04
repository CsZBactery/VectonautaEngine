#include "ECS/Texture.h"

Texture::Texture(const std::string& textureName,
  const std::string& extension)
  : Component(ComponentType::TEXTURE)
  , m_textureName(textureName)
  , m_extension(extension)
{
  std::string path = m_textureName + "." + m_extension;
  if (!m_texture.loadFromFile(path)) {
    std::cerr << "Error loading texture: " << path << std::endl;
  }
  else {
    m_sprite.emplace(m_texture);
  }
}

void Texture::render(const EngineUtilities::TSharedPointer<Window>& window) {
  if (window && m_sprite.has_value()) {
    window->draw(*m_sprite);
  }
}
