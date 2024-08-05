#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

class Wall
{
public:
    Wall(float x, float y)
        : _moveSpeed(-350.f)
    {
        wall.setPosition(x, y);
        wall.setSize(sf::Vector2f(80, 700));

        if (!texture.loadFromFile("res/sprite/pipetest.png"))
        {
            std::cout << "pipe texture failed" << std::endl;
            system("pause");
        }

        wall.setTexture(&texture);
        //
    }

    void update(sf::Time& deltaTime)
    {
        wall.move(_moveSpeed * deltaTime.asSeconds(), 0);
        //
    }

    float getPos() const
    {
        return wall.getPosition().x; //
    }

    void drawTo(sf::RenderWindow& window)
    {
        window.draw(wall); //
    }

    bool collision(const sf::Shape& item) const
    {
        return wall.getGlobalBounds().intersects(item.getGlobalBounds());
    }

private:
    sf::RectangleShape wall;
    static sf::Texture texture;

    float _moveSpeed;
};