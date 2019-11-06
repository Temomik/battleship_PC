#include <SFML/Graphics.hpp>
#include "button.hpp"
#include <vector>

class View
{
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<Button> buttons;
private:

public:
    View(std::string backgroundName);
    ~View();
    void addButton(size_t x, size_t y, size_t width, size_t height, std::string fileName, std::string text,size_t fontSize, std::string font);
    void draw(sf::RenderWindow& window);
    void allignButton(sf::RenderWindow& window);
};

View::View(std::string backgroundName)
{
    texture.loadFromFile(backgroundName);
    sprite.setTexture(texture);
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
}   

void View::addButton(size_t x, size_t y, size_t width, size_t height, std::string fileName, std::string text,size_t fontSize, std::string font)
{
    Button button(x,y,width,height,fileName);
    button.setText(text,fontSize,font);
    buttons.push_back(button);
}