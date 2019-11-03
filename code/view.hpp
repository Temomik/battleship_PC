#include <SFML/Graphics.hpp>

class View
{
    sf::Texture texture;
    sf::Sprite sprite;           
private:

public:
    View(std::string backgroundName);
    ~View();
    void draw(sf::RenderWindow& window);
};

View::View(std::string backgroundName)
{
    texture.loadFromFile(backgroundName);
    sprite.setTexture(texture);
}

View::~View()
{
}

void View::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
