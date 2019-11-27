#include <string>
#include <SFML/Graphics.hpp>
#include "View.hpp"
#include <stack>
#include "Profiles.hpp"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <math.h>

#define DELETE 59

int stringSetNum = -1;
int menuStringSet = -1;
std::string inputString = "";
sf::Event event;
const int gridCount = 10;
const int shipCount = 10;
int ships[] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
const int RadarPrice = 200,artilleryPrice = 300;
int bonus[2] ={0,0};
int waitForDraw = 0;
int startGameConditions = 0;
int rotate = 0;
int currentShipDeck = 0;

// enum menuNames
// {
//     loginMenuEnum,
//     registerMenuEnum,
//     mainMenuEnum,
//     infoMenuEnum,
//     rulesMenuEnum,
//     firstGameStageMenuEnum,
//     secondGameStageMenuEnum,
//     shipSelectMenuEnum,
//     startMenuEnum,
//     bonusMenuEnum
// };

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

bool isEndOfGame(std::vector<int>& data)
{
    bool isEndGameConditions;
    int count = gridCount * gridCount;
    for (size_t i = 0; i < count; i++)
    {
        if (data[i] == 1)
        {
            isEndGameConditions = false;
            break;
        }
        isEndGameConditions = true;
    }
    return isEndGameConditions;
}

int findDirectionsShip(const std::vector<int>& ships,int num)
{
    const int size = sqrt(ships.size());
    if(num + 1 < ships.size() && (ships[num + 1] == 1 || ships[num + 1] == 3) && num % size != size -1)
        return 1;
    if(num - 1 >= 0 && (ships[num - 1] == 1 || ships[num - 1] == 3)&& num % size != 0)
        return -1;
    if(num + size < ships.size() && (ships[num + size] == 1 || ships[num + size] == 3))
        return size;
    if(num - size >= 0 && (ships[num - size] == 1 || ships[num - size] == 3))
        return -size;
    return 0;
}

int findStartShipCord(const std::vector<int>& ships,int num)
{
    int directions = findDirectionsShip(ships,num);
    if(directions == 0)
        return num;
    for(; num < ships.size() && num >= 0 && ( ships[num] == 1 || ships[num] == 3); num += directions);
    return num -= directions;
}

bool isShipAlive(const std::vector<int>& ships, int num)
{
    int size = sqrt(ships.size());
    num = findStartShipCord(ships,num);
    int directions = findDirectionsShip(ships,num);
    if(directions == 0)
    {
        if(ships[num] == 1)
            return false;
        return true;
    }
    while (num >=0 && num < ships.size() && (ships[num] == 1 || ships[num] == 3))
    {
        if(ships[num] == 1)
            return false;
            if((num % size == size- 1 && (num + directions) % size == 0) || (num % size == 0 && (num + directions) % size == size - 1))
            {
                break;
            }
        num+=directions;
    }
    return true;
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

bool makeShoot(int selectedCell, View& view, int gridNum)
{
    if(selectedCell < 0 && selectedCell >= view.getGridData(gridNum).size())
    {
        return true;
    }
    if (view.getGridData(gridNum)[selectedCell] == 0)
    {
        view.setGridData(selectedCell, 2,gridNum);
        view.markCell(selectedCell, "image/water-.jpg",gridNum);
        return false;
    }
    else if (view.getGridData(gridNum)[selectedCell] == 1)
    {
        view.setGridData(selectedCell, 3,gridNum);
        view.markCell(selectedCell, "image/water=.jpg",gridNum);
        if (isShipAlive(view.getGridData(gridNum), selectedCell))
        {
            std::vector<int> allShips = view.getGridData(gridNum);
            int num = findStartShipCord(allShips, selectedCell);
            int directions = findDirectionsShip(allShips, num);
            for (; num >= 0 && num < allShips.size() && (allShips[num] == 1 || allShips[num] == 3); num += directions)
            {
                if ((num % gridCount == 0 && (num + directions) % gridCount == gridCount - 1) || (num % gridCount == gridCount - 1 && (num + directions) % gridCount == 0))
                    break;
                int maxX = 3, maxY = 3;
                int tmpNum;
                if (num >= 0 && num < allShips.size() && (allShips[num] == 1 || allShips[num] == 3))
                {
                    if (num % gridCount == 0)
                    {
                        maxX = 2;
                        tmpNum = num - gridCount;
                    }
                    else
                    {
                        tmpNum = num - 1 - gridCount;
                    }
                    if (tmpNum < 0)
                    {
                        tmpNum += gridCount;
                        maxY = 2;
                    }
                    for (int y = 0; y < maxY; tmpNum += gridCount, y++)
                    {

                        for (int x = 0; x < maxX; x++)
                        {
                            if (tmpNum + x >= 0 && tmpNum + x < allShips.size() && allShips[tmpNum + x] == 0)
                            {
                                view.setGridData(tmpNum + x, 2,gridNum);
                                view.markCell(tmpNum + x, "image/water-.jpg",gridNum);
                            }
                            if ((tmpNum + x) % gridCount == gridCount - 1)
                            {
                                break;
                            }
                        }
                    }
                    if (directions == 0)
                        break;
                }
                else
                {
                    break;
                }
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

void cloneArray(int* first,const int* second,int size)
{
    for (size_t i = 0; i < size; i++)
    {
        first[i] = second[i];
    }
}

void randomShipPlace(const int* ships,View& view, bool vision, int gridNum)
{
    int tmpShips[shipCount];
    cloneArray(tmpShips,ships,shipCount);
    for (int i = 0; i < shipCount; i++)
    {
        int cord, rotate;
        while (1)
        {
            cord = rand() % 100;
            rotate = rand() % 2 - 1;
            if (canPlaceShip(tmpShips[i], cord, gridCount, rotate, view.getGridData(gridNum)))
                break;
        }
        for (int j = 0; j < tmpShips[i]; j++)
            if (rotate == 0)
            {
                if(vision)
                    view.markCell(cord + j, "image/water_.jpg",gridNum);
                view.setGridData(cord + j, 1,gridNum);
           }
            else
            {
                if(vision)
                    view.markCell(cord + j * gridCount, "image/water_.jpg",gridNum);
                view.setGridData(cord + j * gridCount, 1,gridNum);
            }
        tmpShips[i] = -1;
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
View *Menu = &startMenu;
sf::RenderWindow window(sf::VideoMode(1920, 1080), "BattleShip", sf::Style::Fullscreen);
int rightMouseStatus = 0;
int leftMouseStatus = 0;
int middleMouseStatus = 0;
Profiles profiles("saves");
Profile currentUser = {"", "", 0, 0};

void RadarAbility(View& view, int size, int cord, int gridNum)
{
    size--;
    for ( int i = -gridCount; i < size  * gridCount; i+=gridCount)
    {
        for(int j = -1; j < size; j++)
        {
            if(cord+i+j >= 0 && cord+i+j < view.getGridData(gridNum).size())
            {
                if( view.getGridData(gridNum)[cord+i+j] == 1)
                {
                    secondGameStageMenu.markCell(cord+i+j, "image/water_.jpg",gridNum);
                }
            }
        }
    }
}
int handleButtonShipArray[shipCount];
void mainStartButton()
{
    cloneArray(handleButtonShipArray,ships,shipCount);
    int tmpShips[shipCount];
    for (size_t i = 0; i < gridCount * gridCount; i++)
    {
        if(firstGameStageMenu.getGridData(0)[i] != 0)
        {
            firstGameStageMenu.setGridData(i, 0, 0);
            firstGameStageMenu.markCell(i, "image/water.jpg",0);
        }
    }
    cloneArray(tmpShips,ships,shipCount);
    Menu  = &firstGameStageMenu;
}

void mainRulesButton()
{
    Menu = &rulesMenu;
}

void mainQuitButton()
{
    window.close();
}

void mainInfoButton()
{
    infoMenu.setButtonText(0, currentUser.login, 50, "font/consolaz.ttf");
    infoMenu.setButtonText(1, currentUser.password, 50, "font/consolaz.ttf");
    infoMenu.setButtonText(2, toString(currentUser.money), 50, "font/consolaz.ttf");
    infoMenu.setButtonText(3, toString(currentUser.record), 50, "font/consolaz.ttf");
    Menu = &infoMenu;
}

void startRegisterButton()
{

    Menu = &registerMenu;
}
void startLoginButton()
{

    Menu = &loginMenu;
}
void startUnauthorizedButton()
{
    currentUser.money = 6000;
    Menu = &mainMenu;
}

void startQuitButton()
{
    window.close();
}

void registerLoginFieldButton()
{
    inputString = currentUser.login;
    stringSetNum = 2;
}

void registerPasswordFiledButton()
{
    inputString = currentUser.password;
    stringSetNum = 3;
}

void registerRegisterButton()
{
    stringSetNum = -1;
    if (strlen(currentUser.login) > 0 && strlen(currentUser.password) > 0 && !profiles.isConsistLogin(currentUser))
    {
        profiles.addProfile(currentUser);
        Menu = &mainMenu;
    }
    else
    {
        setCharString(currentUser.login, "", MAX_STRINGN);
        setCharString(currentUser.password, "", MAX_STRINGN);
        registerMenu.setButtonText(0, "", 50, "font/consolaz.ttf");
        registerMenu.setButtonText(1, "", 50, "font/consolaz.ttf");
    }
}

void registerBackButton()
{
    {
        Menu = &startMenu;
        stringSetNum = -1;
    }
    stringSetNum = -1;
}

void loginLoginFieldButton()
{
    inputString = currentUser.login;
    stringSetNum = 0;
}

void loginPasswordFieldButton()
{
    inputString = currentUser.password;
    stringSetNum = 1;
}

void loginLoginButton()
{
    stringSetNum = -1;
    if (profiles.isConsistProfile(currentUser))
    {
        currentUser = profiles.getProfile(currentUser);
        // currentUser.money = 6000;
        Menu = &mainMenu;
    }
    else
    {
        setCharString(currentUser.login, "", MAX_STRINGN);
        setCharString(currentUser.password, "", MAX_STRINGN);
        loginMenu.setButtonText(0, "", 50, "font/consolaz.ttf");
        loginMenu.setButtonText(1, "", 50, "font/consolaz.ttf");
    }
}

void loginBackButton()
{
    {
        Menu = &startMenu;
    }
    stringSetNum = -1;
}

void firstGameStageShipsButton()
{
    Menu = &shipSelectMenu;
}
void firstGameStageRotateButton()
{
    rotate ^= 1;
}
void firstGameStageBonusButton()
{
    bonusMenu.setButtonText(0, toString(currentUser.money), 50, "font/consolaz.ttf");
    Menu = &bonusMenu;
}
void firstGameStageRandomButton()
{
    for (size_t i = 0; i < gridCount * gridCount; i++)
    {
        if (firstGameStageMenu.getGridData(0)[i] != 0)
        {
            firstGameStageMenu.setGridData(i, 0, 0);
            firstGameStageMenu.markCell(i, "image/water.jpg", 0);
        }
    }
    randomShipPlace(ships, firstGameStageMenu, true, 0);
    startGameConditions = true;
}
void firstGameStageStartButton()
{
    int tmpShips[shipCount];
    cloneArray(tmpShips,ships,shipCount);
    if (startGameConditions == false)
    {
        startGameConditions = true;
        for (int i = 0; i < shipCount; i++)
        {
            if (tmpShips[i] > 0)
            {
                startGameConditions = false;
                break;
            }
        }
    }
    if (startGameConditions == true)
    {
        secondGameStageMenu.setGridData(firstGameStageMenu.getGridData(0), 1);

        for (int i = 0; i < shipCount * shipCount; i++)
        {
            if (firstGameStageMenu.getGridData(0)[i] != 0)
            {
                secondGameStageMenu.setGridData(firstGameStageMenu.getGridData(0), 1);
                secondGameStageMenu.markCell(i, "image/water_.jpg", 1);
            }
            if (secondGameStageMenu.getGridData(1)[i] != 0)
            {
                secondGameStageMenu.setGridData(i, 0, 1);
                secondGameStageMenu.markCell(i, "image/water.jpg", 0);
            }
        }
        randomShipPlace(ships, secondGameStageMenu, false, 0);
        secondGameStageMenu.setButtonText(0, "Radar " + toString(bonus[0]), 50, "font/consolaz.ttf");
        secondGameStageMenu.setButtonText(1, "Artillery " + toString(bonus[1]), 50, "font/consolaz.ttf");
        Menu = &secondGameStageMenu;
    }
}
void firstGameStageBackButton()
{
    Menu = &mainMenu;
}

void firstGameStageOperation()
{
    startGameConditions = true;
    for (size_t i = 0; i < shipCount; i++)
    {
        if(handleButtonShipArray[i] > 0)
        {
            startGameConditions = false;
            break;
        }
    }
    if(startGameConditions == true)
    {
        return;
    }
    
    int buttonCount = firstGameStageMenu.getSelectedButton(window);
    if (rightMouseStatus == 1)
    {
        rightMouseStatus = 0;
        switch (buttonCount)
        {

            break;
        default:
            break;
        }
        int tmp = firstGameStageMenu.getSelectedCell(window, 0);
        if (tmp >= 0 && canPlaceShip(currentShipDeck, tmp, gridCount, rotate, firstGameStageMenu.getGridData(0)))
        {
            bool isShipAmount = false;
            for (int i = 0; i < shipCount; i++)
            {
                if (handleButtonShipArray[i] == currentShipDeck)
                {
                    handleButtonShipArray[i] = -1;
                    isShipAmount = true;
                    break;
                }
            }
            if (isShipAmount)
                for (int i = 0; i < currentShipDeck; i++)
                {
                    if (rotate == 0)
                    {
                        firstGameStageMenu.markCell(tmp + i, "image/water_.jpg", 0);
                        firstGameStageMenu.setGridData(tmp + i, 1, 0);
                    }
                    else
                    {
                        firstGameStageMenu.markCell(tmp + i * gridCount, "image/water_.jpg", 0);
                        firstGameStageMenu.setGridData(tmp + i * gridCount, 1, 0);
                    }
                }
        }
    }
}

void shipSelectOneButton()
{
    currentShipDeck =  1;
    Menu = &firstGameStageMenu;
}

void shipSelectTwoButton()
{
    currentShipDeck =  2;
    Menu = &firstGameStageMenu;
}

void shipSelectThreeButton()
{
    currentShipDeck =  3;
    Menu = &firstGameStageMenu;
}

void shipSelectFourButton()
{
    currentShipDeck =  4;
    Menu = &firstGameStageMenu;
}

void shipSelectOperation()
{   
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        Menu = &firstGameStageMenu;
    }
    // int buttonCount = shipSelectMenu.getSelectedButton(window);
    // if (rightMouseStatus == 1)
    // {
    //     rightMouseStatus = 0;
    //     currentShipDeck =  buttonCount + 1;
    //     Menu = &firstGameStageMenu;
    // }
    // std::cout << currentShipDeck << std::endl;
}

void bonusRadarButton()
{

    if (currentUser.money > RadarPrice && bonus[0] == 0)
    {
        bonus[0] = 1;
        currentUser.money -= RadarPrice;
        bonusMenu.setButtonText(0, toString(currentUser.money), 50, "font/consolaz.ttf");
    }
}

void bonusArtilleryButton()
{
    if (currentUser.money > artilleryPrice && bonus[1] == 0)
    {
        bonus[1] = 1;
        currentUser.money -= artilleryPrice;
        bonusMenu.setButtonText(0, toString(currentUser.money), 50, "font/consolaz.ttf");
    }
}

void bonusBackButton()
{

    Menu = &firstGameStageMenu;
}

int bonusNumberChoosed = -1;
int isMyStep = true;
bool isEndGameConditions = false;

void secondGameStageRadarButton()
{
    if (bonus[0] != 0)
    {
        bonus[0] = 0;
        bonusNumberChoosed = 0;
        secondGameStageMenu.setButtonText(3, "Radar", 50, "font/consolaz.ttf");
        secondGameStageMenu.setButtonText(0, "Radar 0", 50, "font/consolaz.ttf");
    }
}

void secondGameStageArtilleryButton()
{
    if (bonus[1] != 0)
    {
        bonusNumberChoosed = 1;
        secondGameStageMenu.setButtonText(3, "Artillery", 50, "font/consolaz.ttf");
    }
}

void secondGameStageBackButton()
{
    currentUser.money = bonus[0] * RadarPrice + bonus[1] * RadarPrice;
    Menu = &mainMenu;
    isEndGameConditions = true;
}

void secondGameStageOperation()
{
    static std::stack<int> lastShipsCord;
    if (!isEndGameConditions)
    {
        int selectedCell = secondGameStageMenu.getSelectedCell(window, 0);
        if (rightMouseStatus == 1 && isMyStep == true && selectedCell >= 0)
        {
            if (bonusNumberChoosed >= 0)
            {
                rightMouseStatus = 0;
                bonus[bonusNumberChoosed] = 0;
                RadarAbility(secondGameStageMenu, 3, selectedCell, 0);
                bonusNumberChoosed = -1;
                secondGameStageMenu.setButtonSprite(3, "image/red.jpg");
                secondGameStageMenu.setButtonText(3, "Bot turn", 50, "font/consolaz.ttf");
                isMyStep = false;
            }
            else
            {
                rightMouseStatus = 0;
                waitForDraw = 1;
                if (!makeShoot(selectedCell, secondGameStageMenu, 0))
                {
                    std::cout << "makeshoot" << std::endl;
                    isMyStep = false;
                    secondGameStageMenu.setButtonText(3, "Bot turn", 50, "font/consolaz.ttf");
                    secondGameStageMenu.setButtonSprite(3, "image/red.jpg");
                }
                if (!isShipAlive(secondGameStageMenu.getGridData(0), selectedCell))
                {
                    std::cout << "kill Ship" << std::endl;
                    isEndGameConditions = isEndOfGame(secondGameStageMenu.getGridData(0));
                }
                waitForDraw = 0;
            }
        }
        else
        {
            rightMouseStatus = 0;
        }
        if (isMyStep == false)
        {
            _sleep(300);
            int cord;
            if (lastShipsCord.size() > 0)
            {
                cord = lastShipsCord.top();
                lastShipsCord.pop();
            }
            else
            {
                int max = gridCount * gridCount;
                do
                {
                    cord = rand() % max;
                } while (secondGameStageMenu.getGridData(1)[cord] == 2 || secondGameStageMenu.getGridData(1)[cord] == 3);
            }
            std::cout << lastShipsCord.size() << " " << cord << std::endl;
            if (cord >= 0 && cord < secondGameStageMenu.getGridData(1).size() && secondGameStageMenu.getGridData(1)[cord] == 1)
            {
                if (cord + 1 < secondGameStageMenu.getGridData(1).size() && (secondGameStageMenu.getGridData(1)[cord + 1] == 0 || secondGameStageMenu.getGridData(1)[cord + 1] == 1))
                    lastShipsCord.push(cord + 1);
                if (cord - 1 >= 0 && (secondGameStageMenu.getGridData(1)[cord - 1] == 0 || secondGameStageMenu.getGridData(1)[cord - 1] == 1))
                    lastShipsCord.push(cord - 1);
                if (cord + gridCount < secondGameStageMenu.getGridData(1).size() && (secondGameStageMenu.getGridData(1)[cord + gridCount] == 0 || secondGameStageMenu.getGridData(1)[cord + gridCount] == 1))
                    lastShipsCord.push(cord + gridCount);
                if (cord - gridCount >= 0 && (secondGameStageMenu.getGridData(1)[cord - gridCount] == 0 || secondGameStageMenu.getGridData(1)[cord - gridCount] == 1))
                    lastShipsCord.push(cord - gridCount);
            }
            if (!makeShoot(cord, secondGameStageMenu, 1))
            {
                if (!isShipAlive(secondGameStageMenu.getGridData(1), cord) && secondGameStageMenu.getGridData(1)[cord] == 3)
                {
                    while (lastShipsCord.size() > 0)
                    {
                        lastShipsCord.pop();
                    }
                    isEndGameConditions = isEndOfGame(secondGameStageMenu.getGridData(1));
                }
                isMyStep = true;
                secondGameStageMenu.setButtonText(3, "You turn", 50, "font/consolaz.ttf");
                secondGameStageMenu.setButtonSprite(3, "image/green.jpg");
            }
        }
    } else
    {
        std::cout << "end Game" << std::endl;
        secondGameStageMenu.setButtonText(3, "You kek", 50, "font/consolaz.ttf");
        _sleep(4000);
    }
}

void rulesViewOperation()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        Menu = &mainMenu;
    }
}
void infoViewOperation()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        Menu = &mainMenu;
    }
}

void logicThread()
{
    srand(time(NULL));
    int lastCell = -1;
    int tmpShips[shipCount];
    while (1)
    {

        int buttonNum = Menu->getSelectedButton(window);
        if (rightMouseStatus == 1 && buttonNum >= 0)
        {
            rightMouseStatus = 0;
            Menu->doOperations(buttonNum);
        } else
        {
            Menu->doViewOperation();
            rightMouseStatus = 0;
        }
        _sleep(20);
    }
}

void inputStream()
{
    while (1)
    {
        _sleep(20);
        while (stringSetNum >= 0)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                int code = event.text.unicode;
                if (code < 255)
                {
                    if (code == DELETE)
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

    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Start Game", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Rules", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "info", 50, "font/consolaz.ttf");
    mainMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Quit", 50, "font/consolaz.ttf");
    mainMenu.allignButton(window, 0);
    mainMenu.setButtonOperations(mainStartButton,0);
    mainMenu.setButtonOperations(mainRulesButton,1);
    mainMenu.setButtonOperations(mainInfoButton,2);
    mainMenu.setButtonOperations(mainQuitButton,3);

    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Register", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Login", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "unauthorized", 50, "font/consolaz.ttf");
    startMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Quit", 50, "font/consolaz.ttf");
    startMenu.allignButton(window, 0);
    startMenu.setButtonOperations(startRegisterButton,0);
    startMenu.setButtonOperations(startLoginButton,1);
    startMenu.setButtonOperations(startUnauthorizedButton,2);
    startMenu.setButtonOperations(startQuitButton,3);

    firstGameStageMenu.createGrid(20, 20, 90, gridCount, gridCount, "image/water.jpg");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "ships", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "rotate", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "bonus", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "random", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "start", 50, "font/consolaz.ttf");
    firstGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "back", 50, "font/consolaz.ttf");
    firstGameStageMenu.allignButton(window, 400);
    firstGameStageMenu.setButtonOperations(firstGameStageShipsButton,0);
    firstGameStageMenu.setButtonOperations(firstGameStageRotateButton,1);
    firstGameStageMenu.setButtonOperations(firstGameStageBonusButton,2);
    firstGameStageMenu.setButtonOperations(firstGameStageRandomButton,3);
    firstGameStageMenu.setButtonOperations(firstGameStageStartButton,4);
    firstGameStageMenu.setButtonOperations(firstGameStageBackButton,5);
    firstGameStageMenu.setViewOperation(firstGameStageOperation);

    shipSelectMenu.addButton(0, 0, 500, 150, "image/button.jpg", "one deck", 50, "font/consolaz.ttf");
    shipSelectMenu.addButton(0, 0, 500, 150, "image/button.jpg", "two deck", 50, "font/consolaz.ttf");
    shipSelectMenu.addButton(0, 0, 500, 150, "image/button.jpg", "three deck", 50, "font/consolaz.ttf");
    shipSelectMenu.addButton(0, 0, 500, 150, "image/button.jpg", "four deck", 50, "font/consolaz.ttf");
    shipSelectMenu.allignButton(window, 0);
    shipSelectMenu.setViewOperation(shipSelectOperation);
    shipSelectMenu.setButtonOperations(shipSelectOneButton,0);
    shipSelectMenu.setButtonOperations(shipSelectTwoButton,1);
    shipSelectMenu.setButtonOperations(shipSelectThreeButton,2);
    shipSelectMenu.setButtonOperations(shipSelectFourButton,3);

    infoMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    infoMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    infoMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    infoMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    infoMenu.allignButton(window, 0);
    infoMenu.setViewOperation(infoViewOperation);

    rulesMenu.setViewOperation(rulesViewOperation);

    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "register", 50, "font/consolaz.ttf");
    registerMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Back", 50, "font/consolaz.ttf");
    registerMenu.allignButton(window, 0);
    registerMenu.setButtonOperations(registerLoginFieldButton,0);
    registerMenu.setButtonOperations(registerPasswordFiledButton,1);
    registerMenu.setButtonOperations(registerRegisterButton,2);
    registerMenu.setButtonOperations(registerBackButton,3);

    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "login", 50, "font/consolaz.ttf");
    loginMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Back", 50, "font/consolaz.ttf");
    loginMenu.allignButton(window, 0);
    loginMenu.setButtonOperations(loginLoginFieldButton,0);
    loginMenu.setButtonOperations(loginPasswordFieldButton,1);
    loginMenu.setButtonOperations(loginLoginButton,2);
    loginMenu.setButtonOperations(loginBackButton,3);

    bonusMenu.addButton(0, 0, 500, 150, "image/button.jpg", "", 50, "font/consolaz.ttf");
    bonusMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Radar", 50, "font/consolaz.ttf");
    bonusMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Artillery", 50, "font/consolaz.ttf");
    bonusMenu.addButton(0, 0, 500, 150, "image/button.jpg", "back", 50, "font/consolaz.ttf");
    bonusMenu.allignButton(window,0);
    bonusMenu.setButtonOperations(bonusRadarButton,1);
    bonusMenu.setButtonOperations(bonusArtilleryButton,2);
    bonusMenu.setButtonOperations(bonusBackButton,3);

    secondGameStageMenu.createGrid(20, 20, 90, gridCount, gridCount, "image/water.jpg");
    secondGameStageMenu.createGrid(1600, 20, 30, gridCount, gridCount, "image/water.jpg");
    secondGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Radar", 50, "font/consolaz.ttf");
    secondGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "Artillery", 50, "font/consolaz.ttf");
    secondGameStageMenu.addButton(0, 0, 500, 150, "image/button.jpg", "back", 50, "font/consolaz.ttf");
    secondGameStageMenu.addButton(0, 0, 300, 100, "image/green.jpg", "You turn", 50, "font/consolaz.ttf");
    secondGameStageMenu.allignButton(window, 300);
    secondGameStageMenu.setButtonOperations(secondGameStageRadarButton,0);
    secondGameStageMenu.setButtonOperations(secondGameStageArtilleryButton,1);
    secondGameStageMenu.setButtonOperations(secondGameStageBackButton,2);
    secondGameStageMenu.setViewOperation(secondGameStageOperation);

    window.setFramerateLimit(90);
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

        if (waitForDraw == 0)
        {
            window.clear();
            Menu->draw(window);
            window.display();
        }
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