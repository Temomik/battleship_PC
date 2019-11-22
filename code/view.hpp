#pragma once

#include <SFML/Graphics.hpp>
#include "button.hpp"
#include <vector>
#include "grid.hpp"

const int MaxButtonFunctions = 10;

class View
{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<Button> buttons;
    std::vector<Grid> grids;
    void (*operations[MaxButtonFunctions])() ;
    // Grid grid, enemyGrid;

public:
    View(std::string backgroundName);
    ~View();
    void addButton(size_t x, size_t y, size_t width, size_t height, std::string fileName, std::string text, size_t fontSize, std::string font);
    void createGrid(int x, int y, int size, int width, int height, std::string filename);
    // void createEnemyGrid(int x, int y, int size, int width, int height, std::string filename);
    void draw(sf::RenderWindow &window);
    int getSelectedButton(sf::RenderWindow &window);
    void allignButton(sf::RenderWindow &window, int shift);
    void markSelectedCell(sf::RenderWindow &window, std::string texture, int num);
    void markCell(int num, std::string texture, int gridNum);
    std::vector<int> &getGridData(int num);
    std::vector<Button> &getGrid(int num);
    int getSelectedCell(sf::RenderWindow &window, int num);
    void setGridData(int num, int data,int gridNum);
    void setButtonSprite(int buttonNum,std::string filename);
    void setGridData(std::vector<int>& data,int gridNum);
    void setButtonText(size_t numb, std::string text, size_t size, std::string font);
};

View::View(std::string backgroundName)
{
    texture.loadFromFile(backgroundName);
    sprite.setTexture(texture);
}

std::vector<Button> &View::getGrid(int num)
{
    return this->grids[num].getGrid();
}

void View::setButtonSprite(int buttonNum,std::string filename)
{
    buttons[buttonNum].setTexture(filename,buttons[buttonNum].getRect().left,buttons[buttonNum].getRect().top);
}

std::vector<int> &View::getGridData(int num)
{
    return grids[num].getData();
}

void View::setGridData(int num, int data,int gridNum)
{
    grids[gridNum].setData(num, data);
}

void View::setGridData(std::vector<int>& data,int gridNum)
{
    this->grids[gridNum].setData(data); 
}

int View::getSelectedCell(sf::RenderWindow &window, int num)
{
    return grids[num].getSelectedCell(window);
}

void View::markSelectedCell(sf::RenderWindow &window, std::string texture, int num)
{
    grids[num].markSelectedCell(window, texture);
}

void View::markCell(int num, std::string texture, int gridNum)
{
    grids[gridNum].markCell(num, texture);
}


void View::createGrid(int x, int y, int size, int width, int height, std::string filename)
{
    Grid grid;
    grid.create(x, y, size, width, height, filename);
    grids.push_back(grid);
}


void View::allignButton(sf::RenderWindow &window, int shift)
{
    int maxHeightFill = 0;
    for (auto it : buttons)
    {
        maxHeightFill += it.getRect().height;
    }
    const int verticalShift = (window.getSize().y - maxHeightFill) / (buttons.size() + 1);
    if (verticalShift < 0)
        return;
    int windowWidth = window.getSize().x / 2;
    int verticalAllign = 0;
    for (int i = 0; i < buttons.size(); i++)
    {
        verticalAllign += verticalShift;
        buttons[i].setPosition(windowWidth - buttons[i].getRect().width / 2 + shift, verticalAllign);
        verticalAllign += buttons[i].getRect().height;
    }
}

View::~View()
{
}

void View::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
    for (auto it : buttons)
    {
        it.draw(window);
    }
    for(auto it : grids)
    {
        it.draw(window);
    }
}

void View::setButtonText(size_t numb, std::string text, size_t size, std::string font)
{
    buttons[numb].setText(text, size, font);
}

int View::getSelectedButton(sf::RenderWindow &window)
{
    int buttonNum = -1;
    for (int i = 0; i < buttons.size(); i++)
    {
        if (buttons[i].isButtonSelect(window))
        {
            buttonNum = i;
        }
    }
    return buttonNum;
}

void View::addButton(size_t x, size_t y, size_t width, size_t height, std::string fileName, std::string text, size_t fontSize, std::string font)
{
    Button button(x, y, width, height, fileName);
    button.setText(text, fontSize, font);
    buttons.push_back(button);
}