#include <SFML/Graphics.hpp>
#include <vector>
#include "button.hpp"
#include <string>

class Grid
{
private:
    std::vector<Button> buttons;
    std::vector<int> data;
public:
    Grid(){};
    Grid(int x,int y,int size,int width,int height,std::string filename);
    Grid(const Grid& grid);
    void create(int x,int y,int size,int width,int height,std::string filename);
    void draw(sf::RenderWindow& window);
    int getSelectedCell(sf::RenderWindow& window);
    void setData(std::vector<int>& data);
    void setGrid(std::vector<Button>& grid);
    void markSelectedCell(sf::RenderWindow& window,std::string texture);
    void markCell(int num,std::string texture);
    void markUnselectedCell(sf::RenderWindow& window,std::string texture);
    std::vector<int>& getData();
    std::vector<Button>& getGrid();
    void setData(int num,int data);
    ~Grid();
};

Grid::Grid(int x,int y,int size,int width,int height, std::string filename)
{
    // for (size_t i = 0; i < height; i++)
    // {
    //     for (size_t j = 0,tmpX = x; j < width; j++)
    //     {
    //         buttons.push_back(Button(tmpX,y,size,size,filename));
    //         tmpX += size;
    //     }
    //     y += size;
    // }
}

Grid::Grid(const Grid& grid)
{
    this->data = grid.data;
    this->buttons = grid.buttons;
}

std::vector<Button>& Grid::getGrid()
{
    return this->buttons;
}

void Grid::setData(std::vector<int>& data)
{
    this->data = data;
}

void Grid::setGrid(std::vector<Button>& grid)
{
    this->buttons = grid;
}

std::vector<int>& Grid::getData()
{
    return data;
}

int Grid::getSelectedCell(sf::RenderWindow& window)
{
    for(int i = 0; i < buttons.size(); i++)
    {
        if(buttons[i].isButtonSelect(window))
        {
            return i;
        }
    }
    return -1;
}


void Grid::markSelectedCell(sf::RenderWindow& window,std::string texture)
{
    buttons[getSelectedCell(window)].setTexture(texture);
}

void Grid::markCell(int num,std::string texture)
{
    buttons[num].setTexture(texture);
}

void Grid::markUnselectedCell(sf::RenderWindow& window,std::string texture)
{
    for(int i = 0; i < buttons.size(); i++)
    {
        if(!buttons[i].isButtonSelect(window))
        {
            buttons[i].setTexture(texture);
        }
    }
}

void Grid::setData(int num,int data)
{
        this->data[num] = data;
}

void Grid::create(int x,int y,int size,int width,int height,std::string filename)
{
    for(int i = 0;i < width*height;i++)
    {
        data.push_back(0);
        buttons.push_back(Button(0,0,size,size,filename));
    }
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0, tmpX = x; j < width; j++)
        {
            buttons[height*i+j].setPosition(tmpX,y);
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
