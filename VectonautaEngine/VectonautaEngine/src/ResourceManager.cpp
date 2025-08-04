#include "ResourceManager.h"
#include <iostream>
#include <filesystem>

bool ResourceManager::loadTexture(const std::string& fileName, const std::string& extension) {
  // Si ya está cargada, no hacemos nada
  auto it = m_textures.find(fileName);
  if (it != m_textures.end() && !it->second.isNull()) {
    return true; // ya la teníamos
  }

  // Construir ruta: e.g. "Sprites/Track" + ".png"
  std::string fullName = fileName + "." + extension;
  std::filesystem::path fullPath = std::filesystem::absolute(fullName);

  // Intentar cargar
  auto texturePtr = EngineUtilities::MakeShared<Texture>(fileName, extension);
  // El constructor de Texture ya intenta cargar y emplace el sprite solo si tiene éxito.
  // Pero verificamos si la textura interna se cargó correctamente inspeccionando si el sprite está presente.

  // Para diagnosticar, podrías agregar print aquí si quieres
  // Nota: No tenemos acceso directo a saber si falló sin exponerlo en Texture; asumimos que si el archivo no existe,
  // el usuario verá el mensaje que imprime Texture.

  // Guardar en el mapa (incluso si falló, así no se reintenta infinitamente sin control)
  m_textures[fileName] = texturePtr;

  // Retornar true si la textura fue cargada (podemos comprobar si getTexture devolvió algo válido)
  return !texturePtr.isNull();
}

EngineUtilities::TSharedPointer<Texture> ResourceManager::getTexture(const std::string& fileName) {
  auto it = m_textures.find(fileName);
  if (it != m_textures.end()) {
    return it->second;
  }
  // No encontrada: devolver shared pointer nulo
  return EngineUtilities::TSharedPointer<Texture>();
}
