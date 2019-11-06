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
    void draw(sf::RenderWindow& window);
    void allignButton(sf::RenderWindow& window);
};

View::View(std::string backgroundName)
{
    // buttons.push_back(Button(0,0,400,200,"image/button.jpg"));
    // buttons.push_back(Button(0,0,400,200,"image/button.jpg"));
    // buttons.push_back(Button(0,0,400,200,"image/button.jpg"));
    // buttons.push_back(Button(0,0,400,100,"image/button.jpg"));
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
