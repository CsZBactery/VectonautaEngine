#include "BaseApp.h"

/**
 * @file main.cpp
 * @brief Entry point of the application.
 */

 /**
  * @brief Main function that initializes and runs the application.
  *
  * Creates an instance of the BaseApp class and calls its run method to start the application loop.
  *
  * @return int Exit status of the application. Returns 0 on successful execution.
  */
int
main() {
  BaseApp app;
  return app.run();
}