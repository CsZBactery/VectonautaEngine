#pragma once

#include <utility> // std::exchange, std::swap
#include <type_traits>

namespace EngineUtilities {

  template<typename T>
  class TSharedPointer
  {
  public:
    // Constructor por defecto
    TSharedPointer() noexcept : ptr(nullptr), refCount(nullptr) {}

    // Desde puntero crudo
    explicit TSharedPointer(T* rawPtr) : ptr(rawPtr), refCount(new int(1)) {}

    // Copia (incrementa refcount)
    TSharedPointer(const TSharedPointer<T>& other) noexcept
      : ptr(other.ptr), refCount(other.refCount)
    {
      if (refCount) ++(*refCount);
    }

    // Movimiento
    TSharedPointer(TSharedPointer<T>&& other) noexcept
      : ptr(std::exchange(other.ptr, nullptr)),
      refCount(std::exchange(other.refCount, nullptr))
    {
    }

    // Conversión estática desde otro tipo convertible
    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    TSharedPointer(const TSharedPointer<U>& other) noexcept
      : ptr(other.ptr), refCount(other.refCount)
    {
      if (refCount) ++(*refCount);
    }

    // Asignación copia
    TSharedPointer<T>& operator=(const TSharedPointer<T>& other) noexcept
    {
      if (this != &other) {
        release_internal();
        ptr = other.ptr;
        refCount = other.refCount;
        if (refCount) ++(*refCount);
      }
      return *this;
    }

    // Asignación movimiento
    TSharedPointer<T>& operator=(TSharedPointer<T>&& other) noexcept
    {
      if (this != &other) {
        release_internal();
        ptr = std::exchange(other.ptr, nullptr);
        refCount = std::exchange(other.refCount, nullptr);
      }
      return *this;
    }

    // Destructor
    ~TSharedPointer()
    {
      release_internal();
    }

    // Desreferenciación
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    // Conversión booleana explícita
    explicit operator bool() const noexcept { return ptr != nullptr; }
    bool isNull() const noexcept { return ptr == nullptr; }

    // Obtener puntero crudo
    T* get() const noexcept { return ptr; }

    // Resetear
    void reset(T* newPtr = nullptr) noexcept
    {
      release_internal();
      if (newPtr) {
        ptr = newPtr;
        refCount = new int(1);
      }
      else {
        ptr = nullptr;
        refCount = nullptr;
      }
    }

    // Swap
    void swap(TSharedPointer<T>& other) noexcept
    {
      std::swap(ptr, other.ptr);
      std::swap(refCount, other.refCount);
    }

    // static_pointer_cast equivalente
    template<typename U>
    TSharedPointer<U> static_pointer_cast() const noexcept
    {
      U* casted = static_cast<U*>(ptr);
      if (casted) {
        return TSharedPointer<U>(casted, refCount);
      }
      return TSharedPointer<U>();
    }

    // dynamic_pointer_cast equivalente
    template<typename U>
    TSharedPointer<U> dynamic_pointer_cast() const noexcept
    {
      U* casted = dynamic_cast<U*>(ptr);
      if (casted) {
        return TSharedPointer<U>(casted, refCount);
      }
      return TSharedPointer<U>();
    }

  private:
    // Constructor interno usado por castings para compartir refCount sin incrementarlo dos veces
    TSharedPointer(T* rawPtr, int* existingRefCount) noexcept
      : ptr(rawPtr), refCount(existingRefCount)
    {
      if (refCount) ++(*refCount);
    }

    void release_internal() noexcept
    {
      if (refCount) {
        if (--(*refCount) == 0) {
          delete ptr;
          delete refCount;
        }
        ptr = nullptr;
        refCount = nullptr;
      }
    }

    T* ptr;
    int* refCount;

    // Hacer accesibles los miembros en el constructor de conversión de otro tipo
    template<typename> friend class TSharedPointer;
  };

  /**
   * @brief Crea un TSharedPointer con perfect forwarding.
   */
  template<typename T, typename... Args>
  TSharedPointer<T> MakeShared(Args&&... args)
  {
    return TSharedPointer<T>(new T(std::forward<Args>(args)...));
  }

} // namespace EngineUtilities
