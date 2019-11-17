#include <string>
#include <SFML/Graphics.hpp>
#include "View.hpp"
#include <stack>
#include "Profiles.hpp"
#include <iostream>
#include <conio.h>

int menuNum = 0;
int menuStringSet = -1;
int stringSetNum = -1;
std::string inputString = "";
sf::Event event;

int toAnsi(int code)
{
    int utf32[] = {59, 54, 27, 28, 29, 30, 31, 32, 33, 34, 35, 26, 56, 55, 16, 22, 4, 17, 19, 24, 20, 8, 14, 15, 46, 47, 0, 18, 3, 5, 6, 7, 9, 10, 11, 48, 51, 53, -1, 25, 23, 2, 21, 1, 13, 12, 49, 50, 52, 57};
    int ansi[] = {8, 96, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 61, 113, 119, 101, 114, 116, 121, 117, 105, 111, 112, 91, 93, 97, 115, 100, 102, 103, 104, 106, 107, 108, 59, 39, 92, 92, 122, 120, 99, 118, 98, 110, 109, 44, 46, 47, 32};
    int size = sizeof(utf32) / sizeof(int);
    for (int i = 0; i < size; i++)
    {
        if (utf32[i] == code)
            return ansi[i];
    }
    return -1;
}

void setCharString(char* first,std::string second, int max)
{
    if(max > second.size())
    {
        for(int i = 0; i < second.size(); i++)
        {
            first[i] = second[i];
        }
        first[second.size()] = NULL;
    }
}

void inputStream()
{
    while (1)
    {
        while (stringSetNum >= 0)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                // char buffer[50];
                // itoa(event.text.unicode,buffer,10);
                // inputString = buffer + ' ';
                int code = event.text.unicode;
                if (code < 255)
                {
                    if (code == 59)
                    {
                        if (inputString.size() > 0)
                            inputString = inputString.substr(0, inputString.size() - 1);
                    }
                    else if (inputString.size() < MAX_STRINGN && toAnsi(code) >= 0)
                        inputString += toAnsi(code);
                }
            }
        }
    }
}

int main()
{
    Profile newUser = {"","",0,0}, currentUser = {"","",0,0};
    std::stack<int> lastMenu;
    sf::Thread input(&inputStream);
    input.launch();

    Profiles profiles("saves");
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "BattleShip", sf::Style::Fullscreen);

    View mainMenu("image/background.jpg");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Start Game", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Rules", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "info", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Quit", 50, "font/consolaz.ttf");
    mainMenu.allignButton(window);

    View startMenu("image/background.jpg");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Register", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Login", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "unauthorized", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Quit", 50, "font/consolaz.ttf");
    startMenu.allignButton(window);

    View registerMenu("image/background.jpg");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "register", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Back", 50, "font/consolaz.ttf");
    registerMenu.allignButton(window);

    View loginMenu("image/background.jpg");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "login", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Back", 50, "font/consolaz.ttf");
    loginMenu.allignButton(window);

    window.setFramerateLimit(30);
    int mouseStatus = 0;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased)
                if (event.mouseButton.button == sf::Mouse::Left)
                    mouseStatus = 1;
        }

        switch (stringSetNum)
        {
        case 0:
            setCharString( currentUser.login,inputString,MAX_STRINGN);
            loginMenu.setButtonText(0, inputString, 50, "font/consolaz.ttf");
            break;
        case 1:
            setCharString( currentUser.password,inputString,MAX_STRINGN);
            loginMenu.setButtonText(1, inputString, 50, "font/consolaz.ttf");
            break;
        case 2:
            setCharString( newUser.login,inputString,MAX_STRINGN);
            registerMenu.setButtonText(0, inputString, 50, "font/consolaz.ttf");
            break;
        case 3:
            setCharString( newUser.password,inputString,MAX_STRINGN);
            registerMenu.setButtonText(1, inputString, 50, "font/consolaz.ttf");
            break;
        case 4:
            break;
        default:
            break;
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
        }
        else if (menuNum == 0)
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
                    menuNum = 3;
                    break;
                case 3:
                    window.close();
                    break;
                default:
                    lastMenu.pop();
                    break;
                }
            }
        }
        else if (menuNum == 1)
        {
            int buttonCount = registerMenu.getSelectedButton(window);

            if (mouseStatus == 1)
            {
                mouseStatus = 0;
                switch (buttonCount)
                {
                case 0:
                    inputString = newUser.login;
                    stringSetNum = 2;
                    break;
                case 1:
                    inputString = newUser.password;
                    stringSetNum = 3;
                    break;
                case 2:
                    stringSetNum = -1;
                    if(strlen(newUser.login) > 0 && strlen(newUser.password) > 0 && !profiles.isConsist(newUser))
                    {
                        profiles.addProfile(newUser);
                        menuNum = 3;
                    }else
                    {
                        setCharString(newUser.login,"",MAX_STRINGN);
                        setCharString(newUser.password,"",MAX_STRINGN);
                        registerMenu.setButtonText(0, "", 50, "font/consolaz.ttf");
                        registerMenu.setButtonText(1, "", 50, "font/consolaz.ttf");
                    }
                    break;
                case 3:
                    if (!lastMenu.empty())
                    {
                        menuNum = lastMenu.top();
                        lastMenu.pop();
                        stringSetNum = -1;
                    }
                    stringSetNum = -1;
                    break;
                default:
                    break;
                }
            }
        }
        else if (menuNum == 2)
        {
            int buttonCount = loginMenu.getSelectedButton(window);
            if (mouseStatus == 1)
            {
                mouseStatus = 0;
                switch (buttonCount)
                {
                case 0:
                    inputString = currentUser.login;
                    stringSetNum = 0;
                    break;
                case 1:
                    inputString = currentUser.password;
                    stringSetNum = 1;
                    break;
                case 2:
                    stringSetNum = -1;
                    if(profiles.isConsist(currentUser))
                    {
                        menuNum = 3;    
                    } else
                    {
                        setCharString(currentUser.login,"",MAX_STRINGN);
                        setCharString(currentUser.password,"",MAX_STRINGN);
                        loginMenu.setButtonText(0, "", 50, "font/consolaz.ttf");
                        loginMenu.setButtonText(1, "", 50, "font/consolaz.ttf");
                    }
                    break;
                case 3:
                    if (!lastMenu.empty())
                    {
                        menuNum = 1;
                        menuNum = lastMenu.top();
                        lastMenu.pop();
                    }
                    stringSetNum = -1;
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
    input.terminate();
    input.wait();
    profiles.write("saves");
    return 0;
}