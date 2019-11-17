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
    Grid grid;
public:
    View(std::string backgroundName);
    ~View();
    void addButton(size_t x, size_t y, size_t width, size_t height, std::string fileName, std::string text,size_t fontSize, std::string font);
    void createGrid(int x,int y,int size,int width,int height,std::string filename);
    void draw(sf::RenderWindow& window);
    int getSelectedButton(sf::RenderWindow& window);
    void allignButton(sf::RenderWindow& window);
    void setButtonText(size_t numb, std::string text,size_t size,std::string font);
};

View::View(std::string backgroundName)
{
    texture.loadFromFile(backgroundName);
    sprite.setTexture(texture);
}

void View::createGrid(int x,int y,int size,int width,int height,std::string filename)
{
    grid.create(x,y,size,width,height,filename);
}

void View::allignButton(sf::RenderWindow& window)
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
        buttons[i].setPosition(windowWidth - buttons[i].getRect().width/2,verticalAllign);
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