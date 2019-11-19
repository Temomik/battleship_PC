#pragma once 

#include <SFML/Graphics.hpp>
#include "button.hpp"
#include <vector>
#include "grid.hpp"

class View
{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<Button> buttons;
    Grid grid,enemyGrid;
public:
    View(std::string backgroundName);
    ~View();
    void addButton(size_t x, size_t y, size_t width, size_t height, std::string fileName, std::string text,size_t fontSize, std::string font);
    void createGrid(int x,int y,int size,int width,int height,std::string filename);
    void createEnemyGrid(int x,int y,int size,int width,int height,std::string filename);
    void draw(sf::RenderWindow& window);
    int getSelectedButton(sf::RenderWindow& window);
    void allignButton(sf::RenderWindow& window,int shift);
    void markSelectedCell(sf::RenderWindow& window, std::string texture);
    void markCell(int num, std::string texture);
    void markEnemyCell(int num, std::string texture);
    std::vector<int>& getGridData();
    std::vector<Button>& getGrid();
    int getSelectedCell(sf::RenderWindow& window);
    void setEnemyGridData(std::vector<int>& data);
    void setEnemyGrid(std::vector<Button>& grid);
    void setGridData(int num,int data);
    void setButtonText(size_t numb, std::string text,size_t size,std::string font);
};

View::View(std::string backgroundName)
{
    texture.loadFromFile(backgroundName);
    sprite.setTexture(texture);
}

std::vector<Button>& View::getGrid()
{
    return this->grid.getGrid();
}

void View::setEnemyGrid(std::vector<Button>& grid)
{
    enemyGrid.setGrid(grid);
}

std::vector<int>& View::getGridData()
{
    return grid.getData();
}

void View::setGridData(int num,int data)
{
    grid.setData(num,data);
}

void View::setEnemyGridData(std::vector<int>& data)
{
    enemyGrid.setData(data);
}   
  
int View::getSelectedCell(sf::RenderWindow& window)
{
    return grid.getSelectedCell(window);
}

void View::markSelectedCell(sf::RenderWindow& window, std::string texture)
{
    grid.markSelectedCell(window,texture);
}

void View::markCell(int num, std::string texture)
{
    grid.markCell(num,texture);
}
void View::markEnemyCell(int num, std::string texture)
{
    enemyGrid.markCell(num,texture);
}
void View::createGrid(int x,int y,int size,int width,int height,std::string filename)
{
    grid.create(x,y,size,width,height,filename);
}
void View::createEnemyGrid(int x,int y,int size,int width,int height,std::string filename)
{
    enemyGrid.create(x,y,size,width,height,filename);
}
void View::allignButton(sf::RenderWindow& window,int shift)
{
    int maxHeightFill = 0;
    for(auto it : buttons)
    {
        maxHeightFill += it.getRect().height;
    }
    const int verticalShift = (window.getSize().y - maxHeightFill) / (buttons.size() + 1);
    if(verticalShift < 0)
        return;
    int windowWidth = window.getSize().x/2;
    int verticalAllign = 0; 
    for(int i = 0; i < buttons.size();i++) {
        verticalAllign += verticalShift;
        buttons[i].setPosition(windowWidth - buttons[i].getRect().width/2 + shift,verticalAllign);
        verticalAllign += buttons[i].getRect().height;
    }
}

View::~View()
{
}

void View::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    for(auto it : buttons)
    {
        it.draw(window);
    }
    grid.draw(window);
    enemyGrid.draw(window);
}   

void View::setButtonText(size_t numb, std::string text,size_t size,std::string font)
{
    buttons[numb].setText(text,size,font);
}

int View::getSelectedButton(sf::RenderWindow& window)
{
    int buttonNum = -1;
    for(int i = 0; i < buttons.size(); i++)
    {
        if(buttons[i].isButtonSelect(window))
        {
            buttonNum = i;
        }
    }
    return buttonNum;
}


void View::addButton(size_t x, size_t y, size_t width, size_t height, std::string fileName, std::string text,size_t fontSize, std::string font)
{
    Button button(x,y,width,height,fileName);
    button.setText(text,fontSize,font);
    buttons.push_back(button);
}