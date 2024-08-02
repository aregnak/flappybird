#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Wall
{
public:
    Wall(float x, float y)
        : _moveSpeed(-350.f)
    {
        wall.setPosition(x, y);
        wall.setSize(sf::Vector2f(60, 700));
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

private:
    sf::RectangleShape wall;
    // sf::RectangleShape wall2;

    float _moveSpeed;
};