#include "BaseApp.h"
#include <iostream>

int main() {
  try {
    BaseApp app;
    int result = app.run();
    if (result != 0) {
      std::cerr << "Application exited with error code: " << result << '\n';
    }
    return result;
  }
  catch (const std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << '\n';
    return -1;
  }
}