#pragma once
/**
 * @file Texture.h
 * @brief Componente que carga y dibuja una textura con un sprite de SFML (SFML 3).
 */

#include "Prerequisites.h"
#include "ECS/Component.h"
#include "Window.h"

#include <string>
#include <optional>
#include <SFML/Graphics.hpp> // Sprite, Texture, Angle, etc.

class Window;

/**
 * @class Texture
 * @brief Componente de sprite (textura + sprite) para el Actor.
 *
 * Carga desde: bin/<textureName>.<extension>
 * Ej: ("Sprites/Mario","png") -> bin/Sprites/Mario.png
 */
class Texture : public Component {
public:
  Texture(const std::string& textureName, const std::string& extension = "png");
  ~Texture() override = default;

  void start() override {}
  void update(float /*dt*/) override {}

  // Dibuja el sprite
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  void destroy() override {}

  // Acceso al recurso (por si lo necesitas)
  sf::Texture& getTexture() { return m_texture; }
  const sf::Texture& getTexture() const { return m_texture; }

  // Los llama Actor para sincronizar con Transform
  void setPosition(const sf::Vector2f& p);
  void setRotation(float degrees);           // convierte a sf::Angle internamente
  void setScale(const sf::Vector2f& s);

private:
  sf::Texture               m_texture;   // recurso
  std::optional<sf::Sprite> m_sprite;    // instancia visible (si cargó)
  std::string               m_name;      // ruta base (sin extensión)
  std::string               m_ext;       // "png", etc.
};
