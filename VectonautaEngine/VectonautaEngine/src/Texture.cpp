#include "ECS/Texture.h"
#include "Window.h"
#include <iostream>

// Construye la ruta absoluta esperada para cargar la textura.
// Nota: asumimos que el Working Directory (WD) es $(ProjectDir),
// y que los assets viven bajo "bin/". Ej.: "bin/Sprites/Mario.png".
static std::string makeFullPath(const std::string& base, const std::string& ext) {
  // WD esperado: $(ProjectDir)
  return std::string("bin/") + base + "." + ext;
}

Texture::Texture(const std::string& textureName, const std::string& extension)
  : m_name(textureName), m_ext(extension) {
  // 1) Construye la ruta final y carga el sf::Texture desde disco.
  const std::string path = makeFullPath(m_name, m_ext);
  if (!m_texture.loadFromFile(path)) {
    // Si falla, no hay sprite que dibujar; loguea y sal.
    m_sprite.reset();
    std::cerr << "[Texture] Cannot load: " << path << "\n";
    return;
  }

  // 2) Crea el sprite y le asigna la textura cargada.
  //    Usamos el ctor que recibe la textura (SFML 3) para evitar temporales.
  m_sprite.emplace(m_texture); // ¡l-value, no temporales!

  // 3) Centra el origen del sprite (útil para rotaciones y escalados uniformes).
  const auto bounds = m_sprite->getLocalBounds();
  const auto sz = bounds.size;          // SFML 3: miembro .size (no .size())
  m_sprite->setOrigin(sf::Vector2f{ sz.x * 0.5f, sz.y * 0.5f });
}

// Cambia la posición en coordenadas de mundo.
void Texture::setPosition(const sf::Vector2f& p) {
  if (m_sprite) m_sprite->setPosition(p);
}

// Aplica una rotación en grados. SFML 3 usa sf::Angle → sf::degrees(deg).
void Texture::setRotation(float degrees) {
  if (m_sprite) m_sprite->setRotation(sf::degrees(degrees)); // sf::Angle
}

// Escala no uniforme (x,y).
void Texture::setScale(const sf::Vector2f& s) {
  if (m_sprite) m_sprite->setScale(s);
}

// Dibuja el sprite si existe; si no se cargó la textura, no hace nada.
void Texture::render(const EngineUtilities::TSharedPointer<Window>& window) {
  if (!m_sprite) return;
  window->draw(*m_sprite);
}
