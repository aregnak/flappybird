#include "walls.h"
// rest of includes in walls.h

//sf::Texture Wall::texture;

Wall::Wall(float x, float y)
    : _moveSpeed(-350.f)
{
    wall.setPosition({ x, y });
    wall.setSize(sf::Vector2f(80, 700));

    if (!texture.loadFromFile("res/sprite/pipetest.png"))
    {
        std::cout << "pipe texture failed" << std::endl;
        system("pause");
    }

    wall.setTexture(&texture);
}

const bool Wall::collision(const sf::Shape& item)
{
    // new way of doing it with std::optional in SFML 3.0
    std::optional<sf::FloatRect> checkCollision =
        wall.getGlobalBounds().findIntersection(item.getGlobalBounds());

    if (!checkCollision)
    {
        return false;
    }
    return true;
}