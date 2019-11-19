#include <string>
#include <SFML/Graphics.hpp>
#include "View.hpp"
#include <stack>
#include "Profiles.hpp"
#include <iostream>
#include <conio.h>
#include <ctime>
int stringSetNum = -1;
int menuStringSet = -1;
std::string inputString = "";
sf::Event event;
const int gridCount = 10;
const int shipCount = 10;
int ships[] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
const int RadarPrice = 200,artilleryPrice = 300;
int bonus[2] ={0,0};

enum menuNames
{
    loginMenuEnum,
    registerMenuEnum,
    mainMenuEnum,
    infoMenuEnum,
    rulesMenuEnum,
    firstGameStageMenuEnum,
    secondGameStageMenuEnum,
    shipSelectMenuEnum,
    startMenuEnum,
    bonusMenuEnum
};

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

std::string toString(int num)
{
    char buff[50];
    itoa(num, buff, 10);
    return std::string(buff);
}

bool canPlaceShip(int deck, int cord, int gridCount, int rotate, std::vector<int> &grid)
{
    int k = 0;
    std::vector<std::vector<int>> gridArray;
    std::vector<int> tmp;
    for (size_t i = 0; i < gridCount; i++)
    {
        gridArray.push_back(tmp);
        for (size_t j = 0; j < gridCount; j++)
        {
            gridArray[i].push_back(0);
            gridArray[i][j] = grid[k];
            k++;
        }
    }
    int x = cord % 10, y = cord / 10;
    int maxX, maxY;
    if (rotate == 0)
    {
        if (x + deck > gridCount)
            return false;
        maxX = x + deck;
        maxY = y + 1;
    }
    else
    {
        if (y + deck > gridCount)
            return false;
        maxX = x + 1;
        maxY = y + deck;
    }

    for (int i = y - 1; i <= maxY; i++)
    {
        for (int j = x - 1; j <= maxX; j++)
        {
            if (i >= 0 && j >= 0 && i < gridCount && j < gridCount)
            {
                if (gridArray[i][j] == 1)
                    return false;
            }
        }
    }
    return true;
}

void setCharString(char *first, std::string second, int max)
{
    if (max > second.size())
    {
        for (int i = 0; i < second.size(); i++)
        {
            first[i] = second[i];
        }
        first[second.size()] = NULL;
    }
}
View rulesMenu("image/rules.jpg");
View registerMenu("image/background.jpg");
View loginMenu("image/background.jpg");
View infoMenu("image/background.jpg");
View shipSelectMenu("image/background.jpg");
View firstGameStageMenu("image/background.jpg");
View secondGameStageMenu("image/background.jpg");
View startMenu("image/background.jpg");
View mainMenu("image/background.jpg");
View bonusMenu("image/background.jpg");
sf::RenderWindow window(sf::VideoMode(1920, 1080), "BattleShip", sf::Style::Fullscreen);
int rightMouseStatus = 0;
int leftMouseStatus = 0;
int middleMouseStatus = 0;
int menuNum = startMenuEnum;
Profiles profiles("saves");
    Profile currentUser = {"", "", 0, 0};

void cloneArray(int* first,const int* second,int size)
{
    for (size_t i = 0; i < size; i++)
    {
        first[i] = second[i];
    }
}

void logicThread()
{
    int lastCell = -1;
    int ccurrentShipDeck = 0;
    int rotate = 0;
    std::stack<int> lastMenu;
    int startGameConditions = 0;
    int tmpShips[shipCount];
    while (1)
    {

        if (menuNum == mainMenuEnum)
        {
            int buttonCount = mainMenu.getSelectedButton(window);
            if (rightMouseStatus == 1)
            {
                rightMouseStatus = 0;
                switch (buttonCount)
                {
                case 0:
                    for (size_t i = 0; i < gridCount * gridCount; i++)
                    {
                        firstGameStageMenu.setGridData(i, 0);
                        firstGameStageMenu.markCell( i, "image/water.jpg");
                    }
                    cloneArray(tmpShips,ships,shipCount);
                    menuNum = firstGameStageMenuEnum;
                    break;
                case 1:
                    menuNum = rulesMenuEnum;
                    break;
                case 2:
                    infoMenu.setButtonText(0, currentUser.login, 50, "font/consolaz.ttf");
                    infoMenu.setButtonText(1, currentUser.password, 50, "font/consolaz.ttf");
                    infoMenu.setButtonText(2, toString(currentUser.money), 50, "font/consolaz.ttf");
                    infoMenu.setButtonText(3, toString(currentUser.record), 50, "font/consolaz.ttf");
                    menuNum = infoMenuEnum;
                    break;
                case 3:
                    window.close();
                    break;
                default:
                    break;
                }
            }
        }
        else if (menuNum == startMenuEnum)
        {
            int buttonCount = startMenu.getSelectedButton(window);
            if (rightMouseStatus == 1)
            {
                rightMouseStatus = 0;
                lastMenu.push(menuNum);
                switch (buttonCount)
                {
                case 0:
                    menuNum = registerMenuEnum;
                    break;
                case 1:
                    menuNum = loginMenuEnum;
                    break;
                case 2:
                    menuNum = mainMenuEnum;
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
        else if (menuNum == registerMenuEnum)
        {
            int buttonCount = registerMenu.getSelectedButton(window);

            if (rightMouseStatus == 1)
            {
                rightMouseStatus = 0;
                switch (buttonCount)
                {
                case 0:
                    inputString = currentUser.login;
                    stringSetNum = 2;
                    break;
                case 1:
                    inputString = currentUser.password;
                    stringSetNum = 3;
                    break;
                case 2:
                    stringSetNum = -1;
                    if (strlen(currentUser.login) > 0 && strlen(currentUser.password) > 0 && !profiles.isConsistLogin(currentUser))
                    {
                        profiles.addProfile(currentUser);
                        menuNum = mainMenuEnum;
                    }
                    else
                    {
                        setCharString(currentUser.login, "", MAX_STRINGN);
                        setCharString(currentUser.password, "", MAX_STRINGN);
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
        else if (menuNum == loginMenuEnum)
        {
            int buttonCount = loginMenu.getSelectedButton(window);
            if (rightMouseStatus == 1)
            {
                rightMouseStatus = 0;
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
                    if (profiles.isConsistProfile(currentUser))
                    {
                        currentUser = profiles.getProfile(currentUser);
                        // currentUser.money = 6000;
                        menuNum = mainMenuEnum;
                    }
                    else
                    {
                        setCharString(currentUser.login, "", MAX_STRINGN);
                        setCharString(currentUser.password, "", MAX_STRINGN);
                        loginMenu.setButtonText(0, "", 50, "font/consolaz.ttf");
                        loginMenu.setButtonText(1, "", 50, "font/consolaz.ttf");
                    }
                    break;
                case 3:
                    if (!lastMenu.empty())
                    {
                        menuNum = startMenuEnum;
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
        else if (menuNum == rulesMenuEnum)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                menuNum = mainMenuEnum;
        }
        else if (menuNum == infoMenuEnum)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                menuNum = mainMenuEnum;
        }
        else if (menuNum == firstGameStageMenuEnum)
        {
            int buttonCount = firstGameStageMenu.getSelectedButton(window);
            if (rightMouseStatus == 1)
            {
                rightMouseStatus = 0;
                switch (buttonCount)
                {
                case 0:
                    menuNum = shipSelectMenuEnum;
                    break;
                case 1:
                    rotate ^= 1;
                    break;
                case 2:
                    bonusMenu.setButtonText(0, toString(currentUser.money), 50, "font/consolaz.ttf");
                    menuNum = bonusMenuEnum;
                    break;
                case 3: 
                        for (size_t i = 0; i < gridCount * gridCount; i++)
                        {
                        firstGameStageMenu.setGridData(i, 0);
                        firstGameStageMenu.markCell( i, "image/water.jpg");
                        }
                        cloneArray(tmpShips,ships,shipCount);
                    for (int i = 0; i < shipCount; i++)
                    {
                        int cord, rotate;
                        while (1)
                        {
                            cord = rand() % 100;
                            rotate = rand() % 2 - 1;
                            if (canPlaceShip(tmpShips[i], cord, gridCount, rotate, firstGameStageMenu.getGridData()))
                                break;
                        }
                        for (int j = 0; j < tmpShips[i]; j++)
                            if (rotate == 0)
                            {
                                firstGameStageMenu.markCell( cord + j, "image/water_.jpg");
                                firstGameStageMenu.setGridData(cord + j, 1);
                            }
                            else
                            {
                                firstGameStageMenu.markCell( cord + j * gridCount, "image/water_.jpg");
                                firstGameStageMenu.setGridData(cord + j * gridCount, 1);
                            }
                        tmpShips[i] = -1;
                    }
                    break;
                case 4:
                    startGameConditions = true;
                    for(int i = 0; i < shipCount; i++)
                    {
                        if(tmpShips[i] > 0)
                        {
                            startGameConditions = false;
                            break;
                        }
                    }
                    if(startGameConditions == 1)
                    {
                        secondGameStageMenu.setEnemyGridData(firstGameStageMenu.getGridData());
                        
                        for (int i = 0; i < shipCount*shipCount;i++)
                        {
                            if(firstGameStageMenu.getGridData()[i] == 1)
                            {   
                                secondGameStageMenu.markEnemyCell( i,"image/water_.jpg");
                            }
                        }
                        menuNum = secondGameStageMenuEnum;
                    }
                    break;
                case 5:
                    menuNum = mainMenuEnum;
                    break;
                default:
                    break;
                }
                int tmp = firstGameStageMenu.getSelectedCell(window);
                if (tmp >= 0 && canPlaceShip(ccurrentShipDeck, tmp, gridCount, rotate, firstGameStageMenu.getGridData()))
                {
                    bool isShipAmount = false;
                    for (int i = 0; i < shipCount; i++)
                    {
                        if (tmpShips[i] == ccurrentShipDeck)
                        {
                            tmpShips[i] = -1;
                            isShipAmount = true;
                            break;
                        }
                    }
                    if (isShipAmount)
                        for (int i = 0; i < ccurrentShipDeck; i++)
                        {
                            if (rotate == 0)
                            {
                                firstGameStageMenu.markCell( tmp + i, "image/water_.jpg");
                                firstGameStageMenu.setGridData(tmp + i, 1);
                            }
                            else
                            {
                                firstGameStageMenu.markCell( tmp + i * gridCount, "image/water_.jpg");
                                firstGameStageMenu.setGridData(tmp + i * gridCount, 1);
                            }
                        }
                }
            }
        }
        else if (menuNum == shipSelectMenuEnum)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                menuNum = firstGameStageMenuEnum;
            int buttonCount = shipSelectMenu.getSelectedButton(window);
            if (rightMouseStatus == 1)
            {
                rightMouseStatus = 0;
                ccurrentShipDeck = buttonCount + 1;
                menuNum = firstGameStageMenuEnum;
            }
        } else if(menuNum == bonusMenuEnum)
        {
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            //     menuNum = mainMenuEnum;
            int buttonCount = bonusMenu.getSelectedButton(window);
            if (rightMouseStatus == 1)
            {
                rightMouseStatus = 0;
                switch (buttonCount)
                {
                    case 0:
                        // setCharString(currentUser.password, inputString, MAX_STRINGN);
                        break;
                    case 1:
                        if(currentUser.money > RadarPrice && bonus[0] == 0)
                        {
                            bonus[0] = 1;
                            currentUser.money -= RadarPrice;
                            bonusMenu.setButtonText(0, toString(currentUser.money), 50, "font/consolaz.ttf");
                        }    
                        break;
                    case 2:
                        if(currentUser.money > artilleryPrice && bonus[1] == 0)
                        {
                            bonus[1] = 1;
                            currentUser.money -= artilleryPrice;
                            bonusMenu.setButtonText(0, toString(currentUser.money), 50, "font/consolaz.ttf");
                        }   
                        break;
                    case 3:
                        menuNum = firstGameStageMenuEnum;
                        break;
                }
            }
        }else if(menuNum == secondGameStageMenuEnum)
        {
            int isMyStep = true;
            int buttonCount = secondGameStageMenu.getSelectedButton(window);
            if (rightMouseStatus == 1)
            {
                rightMouseStatus = 0;
                switch (buttonCount)
                {
                    case 0:
                        // setCharString(currentUser.password, inputString, MAX_STRINGN);
                        break;
                    case 1:   
                        break;
                    case 2:  
                        break;
                    case 3:
                        currentUser.money = bonus[0] * RadarPrice + bonus[1] * RadarPrice;
                        menuNum = mainMenuEnum;
                        break;
                }
        }
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
    sf::Thread input(&inputStream), logic(&logicThread);
    logic.launch();
    input.launch();
    srand(time(0));

    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Start Game", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Rules", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "info", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Quit", 50, "font/consolaz.ttf");
    mainMenu.allignButton(window, 0);

    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Register", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Login", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "unauthorized", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Quit", 50, "font/consolaz.ttf");
    startMenu.allignButton(window, 0);

    firstGameStageMenu.createGrid(20, 20, 90, gridCount, gridCount, "image/water.jpg");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "ships", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "rotate", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "bonus", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "random", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "start", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "back", 50, "font/consolaz.ttf");
    firstGameStageMenu.allignButton(window, 400);

    shipSelectMenu.addButton(0, 0, 500, 150, "image/button.jpg", "one deck", 50, "font/consolaz.ttf");
    shipSelectMenu.addButton(0, 0, 500, 150, "image/button.jpg", "two deck", 50, "font/consolaz.ttf");
    shipSelectMenu.addButton(0, 0, 500, 150, "image/button.jpg", "three deck", 50, "font/consolaz.ttf");
    shipSelectMenu.addButton(0, 0, 500, 150, "image/button.jpg", "four deck", 50, "font/consolaz.ttf");
    shipSelectMenu.allignButton(window, 0);

    infoMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    infoMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    infoMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    infoMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    infoMenu.allignButton(window, 0);

    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "register", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Back", 50, "font/consolaz.ttf");
    registerMenu.allignButton(window, 0);

    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "login", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Back", 50, "font/consolaz.ttf");
    loginMenu.allignButton(window, 0);

    bonusMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    bonusMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Radar", 50, "font/consolaz.ttf");
    bonusMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Artillery", 50, "font/consolaz.ttf");
    bonusMenu.addButton(0, 0, 500, 150, "image/button.jpg", "back", 50, "font/consolaz.ttf");
    bonusMenu.allignButton(window,0);

    secondGameStageMenu.createGrid(20, 20, 90, gridCount, gridCount, "image/water.jpg");
    secondGameStageMenu.createEnemyGrid(1600, 20, 30, gridCount, gridCount, "image/water.jpg");
    secondGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Radar", 50, "font/consolaz.ttf");
    secondGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Artillery", 50, "font/consolaz.ttf");
    secondGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "back", 50, "font/consolaz.ttf");
    secondGameStageMenu.allignButton(window, 300);

    window.setFramerateLimit(30);
    while (window.isOpen())
    {
        

        switch (stringSetNum)
        {
        case 0:
            setCharString(currentUser.login, inputString, MAX_STRINGN);
            loginMenu.setButtonText(0, inputString, 50, "font/consolaz.ttf");
            break;
        case 1:
            setCharString(currentUser.password, inputString, MAX_STRINGN);
            loginMenu.setButtonText(1, inputString, 50, "font/consolaz.ttf");
            break;
        case 2:
            setCharString(currentUser.login, inputString, MAX_STRINGN);
            registerMenu.setButtonText(0, inputString, 50, "font/consolaz.ttf");
            break;
        case 3:
            setCharString(currentUser.password, inputString, MAX_STRINGN);
            registerMenu.setButtonText(1, inputString, 50, "font/consolaz.ttf");
            break;
        case 4:
            break;
        default:
            break;  
        }
        while (window.pollEvent(event))  
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
                if (event.mouseButton.button == sf::Mouse::Left)
                    rightMouseStatus = 1;
            if (event.type == sf::Event::MouseButtonPressed)
                if (event.mouseButton.button == sf::Mouse::Right)
                    leftMouseStatus = 1;
            if (event.type == sf::Event::MouseButtonPressed)
                if (event.mouseButton.button == sf::Mouse::Middle)
            middleMouseStatus = 1;
        }

        window.clear();
        if (menuNum == mainMenuEnum)
            mainMenu.draw(window);
        if (menuNum == startMenuEnum)
            startMenu.draw(window);
        if (menuNum == registerMenuEnum)
            registerMenu.draw(window);
        if (menuNum == loginMenuEnum)
            loginMenu.draw(window);
        if (menuNum == rulesMenuEnum)
            rulesMenu.draw(window);
        if (menuNum == infoMenuEnum)
            infoMenu.draw(window);
        if (menuNum == firstGameStageMenuEnum)
            firstGameStageMenu.draw(window);
        if (menuNum == shipSelectMenuEnum)
            shipSelectMenu.draw(window);
        if (menuNum == bonusMenuEnum)
            bonusMenu.draw(window);
        if (menuNum == secondGameStageMenuEnum)
            secondGameStageMenu.draw(window);
        window.display();
    }

    input.terminate();
    input.wait();
    logic.terminate();
    logic.wait();
    profiles.update(currentUser);
    std::cout << "kek";
    profiles.write("saves");
    return 0;
}