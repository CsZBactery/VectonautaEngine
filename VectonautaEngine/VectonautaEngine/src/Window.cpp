#include "Window.h"

Window::Window(int width, int height, const std::string& title) {
    //Inicializar la ventana
    m_windowPtr = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), title);
    //m_window = new sf::RenderWindow(sf::VideoMode(witdh, height), title;

    if (!m_windowPtr.isNull()) {
        m_windowPtr->setFramerateLimit(60); //Limitar 60 FPS
        MESSAGE("Window", "window", "Window created successfully");
    }
    else {
        ERROR("Window", "window", "Failed to create window");
    }
}

Window::~Window() {
    m_windowPtr.reset();
    //SAFE_PTR_RELEASE(m_window);
}

void Window::handleEvents() {
    sf::Event event;
    while (m_windowPtr->pollEvent(event)) {
        //Cerrrar la ventana si el usuario lo indica
        if (event.type == sf::Event::Closed)
            m_windowPtr->close();
    }
}

bool Window::isOpen() const {
    if (m_windowPtr) {
        return m_windowPtr->isOpen();
    }
    else {
        ERROR("Window", "isOpen", "Window is null");
        return false;
    }
}

void Window::clear(const sf::Color& color) {
    if (m_windowPtr) {
        m_windowPtr->clear(color);
    }
    else {
        ERROR("Window", "clear", "Window is null");
    }
}

void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
    if (m_windowPtr) {
        m_windowPtr->draw(drawable, states);
    }
    else {
        ERROR("Window", "draw", "Window is null");
    }
}

void Window::display() {
    if (m_windowPtr) {
        m_windowPtr->display();
    }
    else {
        ERROR("Window", "display", "Window is null");
    }
}

void Window::destroy() {
    m_windowPtr.reset();
}
