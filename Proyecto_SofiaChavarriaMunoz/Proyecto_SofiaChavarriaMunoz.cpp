#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;


int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML window");
    CircleShape shape(100.f);
    shape.setFillColor(Color::Green);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}
