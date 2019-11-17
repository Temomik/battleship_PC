#include <SFML/Graphics.hpp>
#include <vector>
#include "button.hpp"
#include <string>

class Grid
{
private:
    std::vector<Button> buttons;
public:
    Grid(){};
    Grid(int x,int y,int size,int width,int height,std::string filename);
    void create(int x,int y,int size,int width,int height,std::string filename);
    void draw(sf::RenderWindow& window);
    int getSelectedCell(sf::RenderWindow& window);
    void markSelectedCell(sf::RenderWindow& window);
    ~Grid();
};

Grid::Grid(int x,int y,int size,int width,int height, std::string filename)
{
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0,tmpX = x; j < width; j++)
        {
            buttons.push_back(Button(tmpX,y,size,size,filename));
            tmpX += size;
        }
        y += size;
    }
}

int Grid::getSelectedCell(sf::RenderWindow& window)
{
    for(int i = 0; i < buttons.size(); i++)
    {
        if(buttons[i].isButtonSelect(window))
        {
            buttons[i] = Button();
        }
    }
}

void Grid::create(int x,int y,int size,int width,int height,std::string filename)
{
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0, tmpX = x; j < width; j++)
        {
            buttons.push_back(Button(tmpX,y,size,size,filename));
            tmpX += size;
        }
        y += size;
    }
}

void Grid::draw(sf::RenderWindow& window)
{
    for(auto it : buttons)
        it.draw(window);
}

Grid::~Grid()
{
}
