#include <SFML/Graphics.hpp>
#include "View.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!", sf::Style::Fullscreen);
    View view("image/background.jpg");
    view.allignButton(window);

    window.setFramerateLimit(30);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        view.draw(window);
        window.display();
    }
    return 0;
}