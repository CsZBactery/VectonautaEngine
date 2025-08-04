#pragma once

#include <utility> // for std::exchange, std::forward

namespace EngineUtilities {

  /**
   * @brief Clase TUniquePtr para manejo exclusivo de memoria.
   *
   * La clase TUniquePtr gestiona la memoria de un objeto de tipo T y garantiza
   * que solo una instancia de TUniquePtr puede poseer y gestionar el objeto en
   * cualquier momento.
   */
  template<typename T>
  class TUniquePtr
  {
  public:
    // Constructor por defecto (nullptr)
    TUniquePtr() noexcept : ptr(nullptr) {}

    // Constructor desde puntero crudo (propiedad)
    explicit TUniquePtr(T* rawPtr) noexcept : ptr(rawPtr) {}

    // Movimiento (transferencia de propiedad)
    TUniquePtr(TUniquePtr<T>&& other) noexcept : ptr(std::exchange(other.ptr, nullptr)) {}

    // Asignación por movimiento
    TUniquePtr<T>& operator=(TUniquePtr<T>&& other) noexcept
    {
      if (this != &other)
      {
        reset(); // libera lo actual
        ptr = std::exchange(other.ptr, nullptr);
      }
      return *this;
    }

    // Eliminar copia
    TUniquePtr(const TUniquePtr<T>&) = delete;
    TUniquePtr<T>& operator=(const TUniquePtr<T>&) = delete;

    // Destructor
    ~TUniquePtr()
    {
      delete ptr;
    }

    // Desreferenciación
    T& operator*() const { return *ptr; }

    // Acceso a miembros
    T* operator->() const { return ptr; }

    // Obtener puntero crudo
    T* get() const noexcept { return ptr; }

    // Liberar propiedad y devolver el puntero sin gestionar
    T* release() noexcept
    {
      return std::exchange(ptr, nullptr);
    }

    // Reiniciar (reemplazar) el contenido
    void reset(T* rawPtr = nullptr) noexcept
    {
      // Intercambia y destruye el anterior de forma segura
      T* old = std::exchange(ptr, rawPtr);
      delete old;
    }

    // Swap
    void swap(TUniquePtr<T>& other) noexcept
    {
      std::swap(ptr, other.ptr);
    }

    // Comprobar si es nulo
    bool isNull() const noexcept
    {
      return ptr == nullptr;
    }

    // Conversión booleana explícita para permitir if(ptr) / if(!ptr)
    explicit operator bool() const noexcept
    {
      return ptr != nullptr;
    }

    // operator! para que !ptr funcione directamente
    bool operator!() const noexcept
    {
      return ptr == nullptr;
    }

  private:
    T* ptr; ///< Puntero al objeto gestionado.
  };

  /**
   * @brief Función de utilidad para crear un TUniquePtr con perfect forwarding.
   *
   * @tparam T Tipo del objeto gestionado.
   * @tparam Args Tipos de los argumentos del constructor del objeto gestionado.
   * @param args Argumentos del constructor del objeto gestionado.
   * @return Un objeto TUniquePtr gestionando un nuevo objeto de tipo T.
   */
  template<typename T, typename... Args>
  TUniquePtr<T> MakeUnique(Args&&... args)
  {
    return TUniquePtr<T>(new T(std::forward<Args>(args)...));
  }

} // namespace EngineUtilities
