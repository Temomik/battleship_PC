#include <SFML/Graphics.hpp>
#include <string>

class Button
{
private:
    sf::Texture texture;
    sf::Sprite sprite;    
    sf::IntRect rect;
public:
    Button(size_t x, size_t y, size_t width, size_t height, std::string fileName);
    Button(const Button& copy);
    void draw(sf::RenderWindow& window);
    void setPosition(int x, int y);
    sf::IntRect getRect();
    ~Button();
};

Button::Button(size_t x, size_t y, size_t width, size_t height, std::string fileName)
{
    texture.loadFromFile(fileName);
    sprite.setTexture(texture);
    sprite.setPosition(x,y);
    rect = sf::IntRect(x,y,width,height);
    sprite.setTextureRect(rect);
}

void Button::draw(sf::RenderWindow& window) 
{
    window.draw(sprite);
}

Button::Button(const Button& copy)
{
    this->texture = copy.texture;
    this->sprite.setTexture(copy.texture);
    rect = copy.rect;
    this->sprite.setTextureRect(copy.rect);
    this->sprite.setPosition(copy.sprite.getPosition().x,copy.sprite.getPosition().y);
}

void Button::setPosition(int x, int y)
{
    this->sprite.setPosition(x,y);
}

sf::IntRect Button::getRect()
{
    return rect;
}

Button::~Button()
{
}


