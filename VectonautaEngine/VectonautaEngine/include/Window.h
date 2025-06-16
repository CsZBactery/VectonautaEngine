#pragma once
#include "Prerequisites.h"


class
Window {
public:
	Window() = default;
	Window(int witdh, int height, const std::string& title);
	~Window();

	void
	handleEvents();

	bool
	isOpen() const;

	void
	clear(const sf::Color& color = sf::Color(0, 0, 0, 255));

	void
	draw(const sf::Drawable& drawable, 
		 const sf::RenderStates& states = sf::RenderStates::Default);

	void
	display();

	void
	destroy();
    private:
		EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr;
	//sf::RenderWindow* m_window;
	sf::View m_view; 
};