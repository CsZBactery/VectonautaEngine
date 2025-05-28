#include <SFML/Graphics.hpp>

int main()
{
    // Crear la ventana
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ejemplo simple de SFML");

    // Crear un círculo
    sf::CircleShape circle(100.f); // radio 100
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition(200.f, 150.f);

    // Bucle principal
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Cerrar la ventana si el usuario lo indica
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Limpiar, dibujar y mostrar
        window.clear();
        window.draw(circle);
        window.display();
    }

    return 0;
}