#include <SFML/Graphics.hpp>
#include <string>

class Button
{
private:
    sf::Texture texture;
    sf::Sprite sprite;    
    sf::IntRect rect;
    sf::Font font;
    sf::Text text;
public:
    Button(size_t x, size_t y, size_t width, size_t height, std::string fileName);
    Button(const Button& copy);
    void draw(sf::RenderWindow& window);
    void setPosition(int x, int y);
    void setText(std::string text,size_t size,std::string font);
    bool isButtonSelect(sf::RenderWindow& window);
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
    window.draw(text);
}

Button::Button(const Button& copy)
{
    this->texture = copy.texture;
    this->sprite.setTexture(copy.texture);
    rect = copy.rect;
    this->sprite.setTextureRect(copy.rect);
    this->sprite.setPosition(copy.sprite.getPosition().x,copy.sprite.getPosition().y);
    this->font = copy.font;
    this->text.setColor(copy.text.getColor());
    this->text.setFont(this->font);
    this->text.setPosition(copy.text.getPosition().x+sprite.getPosition().x,copy.text.getPosition().y+sprite.getPosition().y);
    this->text.setString(copy.text.getString());
    this->text.setCharacterSize(copy.text.getCharacterSize());
}

void Button::setPosition(int x, int y)
{
    this->sprite.setPosition(x,y);
}

sf::IntRect Button::getRect()
{
    return rect;
}

void Button::setText(std::string text,size_t size,std::string font)
{
    this->font.loadFromFile(font);
    this->text.setFont(this->font);
    this->text.setString(text);    
    this->text.setCharacterSize(size);
    
    this->text.setPosition(sprite.getPosition().x + (rect.width/2 - (size*text.size()/2+size/2)/2),sprite.getPosition().y+rect.height/2-size/2);
}

bool Button::isButtonSelect(sf::RenderWindow& window)
{
    int xMouse = sf::Mouse::getPosition(window).x;
    int yMouse = sf::Mouse::getPosition(window).y;
    if (sprite.getPosition().x < xMouse && sprite.getPosition().x + rect.width > xMouse &&
    sprite.getPosition().y < yMouse && sprite.getPosition().y + rect.height > yMouse)
    {
        text.setColor(sf::Color::Red);
        return true;
    } else
    {
        text.setColor(sf::Color::White);
        return false;
    } 
}

Button::~Button()
{
}


