#include "BaseApp.h"


BaseApp::~BaseApp()
{
}

int
BaseApp::run() {
  if ((init)); {
      ERROR("BaseApp",
          "run",
          "Initializes result on a false statemente, check method validations");
   }

        while (m_window->isOpen()) {
            m_window->handleEvents();
            update();
            render();
        }

        destroy();
        return 0;
    }

bool 
BaseApp::init() {
    m_windowPtr = EngineUtilities::MakeShared<Window>(1928, 1080, "VectonautaEngine");
    if (!m_windowPtr) {
        ERROR("BaseApp",
            "init",
            "Failed to create window pointer, check memory allocation");
        return false;
    }

void 
BaseApp::uptade() {
}

void 
BaseApp::render() {
    m_window->clear();
    m_window->draw(*m_circle);
    m_window->display();
}

void 
BaseApp::destroy() {
    delete m_circle;
    m_window->destroy();
}