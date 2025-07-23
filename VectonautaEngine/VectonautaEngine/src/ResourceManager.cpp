#include "ResourceManager.h"
#include "Prerequisites.h"  // para ERROR, MESSAGE, etc.
#include <iostream>         // para std::cerr

bool ResourceManager::loadTexture(const std::string& fileName,
  const std::string& extension)
{
  // 1) Si ya está cargada, devolvemos true inmediatamente
  if (m_textures.find(fileName) != m_textures.end()) {
    return true;
  }

  // 2) Creamos y almacenamos la nueva textura
  auto texture = EngineUtilities::MakeShared<Texture>(fileName, extension);
  m_textures[fileName] = texture;

  // Podrías comprobar aquí si la carga interna de SFML fue exitosa
  // y devolver false en caso contrario.

  return true;
}

EngineUtilities::TSharedPointer<Texture>
ResourceManager::getTexture(const std::string& fileName)
{
  // 1) Intentamos encontrar la textura solicitada
  auto it = m_textures.find(fileName);
  if (it != m_textures.end()) {
    return it->second;
  }

  // 2) Si no existe, avisamos y usamos la textura por defecto
  std::cerr << "[ResourceManager] Texture not found: "
    << fileName << ". Using default texture.\n";

  const std::string defaultKey = "Default";

  // 2a) Si la textura por defecto ya está cargada, la devolvemos
  auto defaultIt = m_textures.find(defaultKey);
  if (defaultIt != m_textures.end()) {
    return defaultIt->second;
  }

  // 2b) Si no, la cargamos y la almacenamos
  auto defaultTexture = EngineUtilities::MakeShared<Texture>(defaultKey, "png");
  m_textures[defaultKey] = defaultTexture;
  return defaultTexture;
}
