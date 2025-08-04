#include "BaseApp.h"
#include <iostream>

int main() {
  BaseApp app;
  int result = app.run();
  if (result != 0) {
    std::cerr << "La aplicación terminó con error: " << result << "\n";
  }
  return result;
}
