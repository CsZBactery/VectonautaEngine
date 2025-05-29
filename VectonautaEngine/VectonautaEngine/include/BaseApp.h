#pragma once
#include "Prerequisites.h"
#include "Window.h"
class
	BaseApp
{
public:
	BaseApp() = default;
	~BaseApp();

	//Funcion encargada de ejecutar la aplicacion en main
	int
		run();

	//Funcion inicializacion 
	bool
		init();

	//Funcion que se actualiza por frame
	void
		uptade();

	//Funcion de renderizado
	void
		render();

	void
		destroy();

private:
	Window* m_window;
//sf::RenderWindow* m_window;
	sf::CircleShape* m_circle;
};