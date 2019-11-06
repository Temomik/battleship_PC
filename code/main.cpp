#include <SFML/Graphics.hpp>
#include "View.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!", sf::Style::Fullscreen);
    View view("image/background.jpg");
    view.addButton(0,0,500,150,"image/button.jpg","Start Game",50,"font/consolaz.ttf");
    view.addButton(0,0,500,150,"image/button.jpg","Rules",50,"font/consolaz.ttf");
    view.addButton(0,0,500,150,"image/button.jpg","info",50,"font/consolaz.ttf");
    view.addButton(0,0,500,150,"image/button.jpg","Quit",50,"font/consolaz.ttf");
    view.allignButton(window);

    window.setFramerateLimit(30);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        view.draw(window);
        window.display();
    }
    return 0;
}