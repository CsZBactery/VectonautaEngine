#include "BaseApp.h"
#include <iostream>

// Punto de entrada de la aplicación.
int main() {
  try {
    // Instancia la aplicación principal.
    BaseApp app;

    // Ejecuta el ciclo principal: init → loop(update/render) → destroy.
    int result = app.run();

    // Si el retorno no es 0, reporta el código de error.
    if (result != 0) {
      std::cerr << "Application exited with error code: " << result << '\n';
    }
    return result; // 0 en éxito, distinto de 0 en error controlado.
  }
  catch (const std::exception& e) {
    // Manejo de excepciones no controladas: log y código de fallo genérico.
    std::cerr << "Unhandled exception: " << e.what() << '\n';
    return -1;
  }
}
