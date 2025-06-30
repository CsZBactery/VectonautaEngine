#include "BaseApp.h"

/**
 * @file BaseApp.cpp
 * @brief Implements the BaseApp class which manages the main application loop.
 */

 /**
  * @brief Destructor for BaseApp.
  *
  * Currently empty, as resources are freed in the destroy() method.
  */
BaseApp::~BaseApp() {
}

/**
 * @brief Starts the main loop of the application.
 *
 * Initializes the application, enters the main loop where events are handled, and updates and renders
 * the application. Finally, it calls destroy() to release resources.
 *
 * @return int Returns 0 on successful execution.
 */
int
BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp", "run", "Initializes result on a false statement",
      "check method validations");
  }

  while (m_windowPtr->isOpen()) {
    m_windowPtr->handleEvents();
    update();
    render();
  }

  destroy();
  return 0;
}

/**
 * @brief Initializes the application resources.
 *
 * Creates the window and sets up a simple yellow circle shape.
 *
 * @return true if initialization succeeds.
 */
bool
BaseApp::init() {
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "Onigiri Engine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window pointer, check memory allocation");
    return false;
  }

  m_shapePtr = EngineUtilities::MakeShared<CShape>();
  if (m_shapePtr) {
    m_shapePtr->createShape(ShapeType::TRIANGLE);
    m_shapePtr->setFillColor(sf::Color::Yellow);
    m_shapePtr->setPosition(200.f, 150.f);
  }

  return true;
}

/**
 * @brief Updates the application state.
 *
 * Empty for now. Intended for game logic or state updates.
 */
void
BaseApp::update() {
}

/**
 * @brief Renders the current frame.
 *
 * Clears the screen, draws the circle shape, and displays the result.
 */
void
BaseApp::render() {
  if (!m_windowPtr) {
    return;
  }
  m_windowPtr->clear();
  if (m_shapePtr) {
    m_shapePtr->render(m_windowPtr);
  }
  m_windowPtr->display();
}

/**
 * @brief Releases allocated resources.
 *
 * Deletes the shape and properly destroys the window.
 */
void
BaseApp::destroy() {
  // m_shapePtr.Reset(); // Not necessary if using smart pointers correctly
  // m_windowPtr.Reset(); // Cleanup handled automatically
}