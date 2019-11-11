#include <SFML/Graphics.hpp>
#include "View.hpp"
#include "stack"

int main()
{
    std::stack<int> lastMenu;

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!", sf::Style::Fullscreen);
    View mainMenu("image/background.jpg");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Start Game", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Rules", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "info", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Quit", 50, "font/consolaz.ttf");
    mainMenu.allignButton(window);

    View startMenu("image/background.jpg");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Register", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Login", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Quit", 50, "font/consolaz.ttf");
    startMenu.allignButton(window);

    View registerMenu("image/background.jpg");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "reg", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Back", 50, "font/consolaz.ttf");
    registerMenu.allignButton(window);

    View loginMenu("image/background.jpg");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "log", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Back", 50, "font/consolaz.ttf");
    loginMenu.allignButton(window);

    int menuNum = 0;
    window.setFramerateLimit(30);
    int mouseStatus = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonReleased)
                if (event.mouseButton.button == sf::Mouse::Left)
                    mouseStatus = 1;
        }
        if (menuNum == 3)
        {
            int buttonCount = mainMenu.getSelectedButton(window);
            if (mouseStatus == 1)
            {
                mouseStatus = 0;
                switch (buttonCount)
                {
                case 0:
                    // window.close();
                    break;
                case 1:
                    // window.close();
                    break;
                case 2:
                    // window.close();
                    break;
                case 3:
                    window.close();
                    break;
                default:
                    break;
                }
            }
        } else
        if (menuNum == 0)
        {
            int buttonCount = startMenu.getSelectedButton(window);

            if (mouseStatus == 1)
            {
                mouseStatus = 0;
                lastMenu.push(menuNum);
                switch (buttonCount)
                {
                case 0:
                    menuNum = 1;
                    break;
                case 1:
                    menuNum = 2;
                    break;
                case 2:
                    window.close(); 
                    break;
                default:
                    lastMenu.pop();
                    break;
                }
            }
        } else 
        if (menuNum == 1)
        {
            int buttonCount = registerMenu.getSelectedButton(window);

            if (mouseStatus == 1)
            {
                mouseStatus = 0;
                switch (buttonCount)
                {
                case 0:
                    // menuNum = 1;
                    break;
                case 1:
                    // menuNum = 2;
                    break;
                case 2:
                    if(!lastMenu.empty())
                    {
                        menuNum = lastMenu.top();
                        lastMenu.pop();
                    }
                    break;
                default:
                    break;
                }
            }
        } else 
        if (menuNum == 2)
        {
            int buttonCount = loginMenu.getSelectedButton(window);
            if (mouseStatus == 1)
            {
                mouseStatus = 0 ;
                switch (buttonCount)
                {
                case 0:
                    // menuNum = 1;
                    break;
                case 1:
                    // menuNum = 2;
                    break;
                case 2:
                    if(!lastMenu.empty())
                    {
                        menuNum = 1;
                        menuNum = lastMenu.top();
                        lastMenu.pop();
                    }
                    break;
                default:
                    break;
                }
            }
        }
        window.clear();
        if (menuNum == 3)
            mainMenu.draw(window);
        if (menuNum == 0)
            startMenu.draw(window);
        if (menuNum == 1)
            registerMenu.draw(window);
        if (menuNum == 2)
            loginMenu.draw(window);
        window.display();
    }
    return 0;
}