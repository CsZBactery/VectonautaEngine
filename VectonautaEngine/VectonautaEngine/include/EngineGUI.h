#pragma once
#include "Prerequisites.h"

//Imgui
//#include <imgui.h>
//#include <imgui-SFML.h>

class Window;

class
  EngineGUI {
public:
  EngineGUI() = default;
  ~EngineGUI() = default;


  void
    init(const EngineUtilities::TSharedPointer<Window>& window);

  void
    update(const EngineUtilities::TSharedPointer<Window>& window,
      sf::Time deltaTime);

  void
    render(const EngineUtilities::TSharedPointer<Window>& window);



  void setupGreyGUIStyle();

  void
    destroy();

  void
    processEvent(const sf::Window& window, const sf::Event& event);

  void
    setupDarkGUIStyle();

  void
    barMenu();


private:

};